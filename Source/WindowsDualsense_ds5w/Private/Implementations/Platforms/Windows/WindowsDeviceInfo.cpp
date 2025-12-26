// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Implementations/Platforms/Windows/WindowsDeviceInfo.h"
#include "GCore/Types/Structs/Config/GamepadCalibration.h"
#include "GCore/Types/Structs/Context/DeviceContext.h"
#include "GImplementations/Utils/GamepadSensors.h"
#include "Helpers/DualSenseLog.h"
#include <filesystem>
#include <hidsdi.h>
#include <setupapi.h>

void FWindowsDeviceInfo::Detect(std::vector<FDeviceContext>& Devices)
{
	Devices.clear();

	GUID HidGuid;
	HidD_GetHidGuid(&HidGuid);
	const HDEVINFO DeviceInfoSet = SetupDiGetClassDevs(&HidGuid, nullptr, nullptr,
	                                                   DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (DeviceInfoSet == INVALID_HANDLE_VALUE)
	{
		return;
	}

	SP_DEVICE_INTERFACE_DATA DeviceInterfaceData = {};
	DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	for (int32 DeviceIndex = 0; SetupDiEnumDeviceInterfaces(DeviceInfoSet, nullptr, &HidGuid, DeviceIndex,
	                                                        &DeviceInterfaceData);
	     DeviceIndex++)
	{
		DWORD RequiredSize = 0;
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, nullptr, 0, &RequiredSize, nullptr);

		const auto DetailDataBuffer = static_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(malloc(RequiredSize));
		if (!DetailDataBuffer)
		{
			UE_LOG(LogTemp, Error, TEXT("HIDManager: Failed to allocate memory for device details."));
			continue;
		}

		DetailDataBuffer->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if (SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, DetailDataBuffer, RequiredSize,
		                                    nullptr, nullptr))
		{
			std::string InPath = std::filesystem::path(DetailDataBuffer->DevicePath).string();
			const HANDLE TempDeviceHandle = CreateFileW(
			    DetailDataBuffer->DevicePath,
			    GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, NULL, nullptr);

			if (TempDeviceHandle != INVALID_HANDLE_VALUE)
			{
				HIDD_ATTRIBUTES Attributes = {};
				Attributes.Size = sizeof(HIDD_ATTRIBUTES);
				if (HidD_GetAttributes(TempDeviceHandle, &Attributes))
				{
					if (
					    Attributes.VendorID == 0x054C &&
					    (Attributes.ProductID == 0x0CE6 ||
					     Attributes.ProductID == 0x0DF2 ||
					     Attributes.ProductID == 0x05C4 ||
					     Attributes.ProductID == 0x09CC))
					{
						FDeviceContext Context = {};
						Context.Handle = nullptr;
						Context.Path = std::filesystem::path(DetailDataBuffer->DevicePath).string();
						switch (Attributes.ProductID)
						{
							case 0x05C4:
							case 0x09CC:
								Context.DeviceType = EDSDeviceType::DualShock4;
								break;
							case 0x0CE6:
								Context.DeviceType = EDSDeviceType::DualSense;
								break;
							case 0x0DF2:
								Context.DeviceType = EDSDeviceType::DualSenseEdge;
								break;
							default:
								Context.DeviceType = EDSDeviceType::NotFound;
						}

						if (Context.DeviceType != EDSDeviceType::NotFound)
						{
							Context.IsConnected = true;
							Context.ConnectionType = EDSDeviceConnection::Usb;
							std::string BluetoothGUID = "{00001124-0000-1000-8000-00805f9b34fb}";
							if (Context.Path.find(BluetoothGUID) != std::string::npos)
							{
								Context.ConnectionType = EDSDeviceConnection::Bluetooth;
							}
							Devices.push_back(Context);
						}
					}
				}
				if (TempDeviceHandle != INVALID_HANDLE_VALUE)
				{
					CloseHandle(TempDeviceHandle);
				}
			}
		}
		free(DetailDataBuffer);
	}
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
}

void FWindowsDeviceInfo::Read(FDeviceContext* Context)
{
	if (!Context)
	{
		return;
	}

	if (Context->Handle == INVALID_PLATFORM_HANDLE)
	{
		return;
	}

	if (!Context->IsConnected)
	{
		return;
	}

	DWORD BytesRead = 0;
	if (Context->ConnectionType == EDSDeviceConnection::Bluetooth && Context->DeviceType == EDSDeviceType::DualShock4)
	{
		constexpr size_t InputReportLength = 547;
		EPollResult Result = PollTick(Context->Handle, Context->BufferDS4, InputReportLength, BytesRead);
		if (Result != EPollResult::ReadOk)
		{
			InvalidateHandle(Context);
		}
	}
	else
	{
		const size_t InputBufferSize = Context->ConnectionType == EDSDeviceConnection::Bluetooth ? 78 : 64;
		EPollResult Result = PollTick(Context->Handle, Context->Buffer, InputBufferSize, BytesRead);
		if (Result != EPollResult::ReadOk)
		{
			InvalidateHandle(Context);
		}
	}
}

void FWindowsDeviceInfo::Write(FDeviceContext* Context)
{
	if (Context->Handle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	size_t InReportLength = Context->DeviceType == EDSDeviceType::DualShock4 ? 32 : 74;
	size_t OutputReportLength = Context->ConnectionType == EDSDeviceConnection::Bluetooth ? 78 : InReportLength;

	DWORD BytesWritten = 0;
	if (!WriteFile(Context->Handle, Context->BufferOutput, OutputReportLength, &BytesWritten, nullptr))
	{
		InvalidateHandle(Context);
	}
}

bool FWindowsDeviceInfo::CreateHandle(FDeviceContext* DeviceContext)
{
	std::string Source = DeviceContext->Path;
	std::wstring MyStdString = std::filesystem::path(Source).wstring();
	const HANDLE DeviceHandle = CreateFileW(
	    MyStdString.data(),
	    GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, NULL, nullptr);

	if (DeviceHandle == INVALID_PLATFORM_HANDLE)
	{
		DeviceContext->Handle = DeviceHandle;
		return false;
	}

	DeviceContext->Handle = DeviceHandle;
	ConfigureFeatures(DeviceContext);
	return true;
}

void FWindowsDeviceInfo::InvalidateHandle(FDeviceContext* Context)
{
	if (!Context)
	{
		return;
	}

	if (Context->Handle != INVALID_PLATFORM_HANDLE)
	{
		CloseHandle(Context->Handle);
		Context->Handle = INVALID_PLATFORM_HANDLE;
		Context->IsConnected = false;
		Context->Path.clear();

		ZeroMemory(Context->BufferOutput, sizeof(Context->BufferOutput));
		ZeroMemory(Context->BufferAudio, sizeof(Context->BufferAudio));
		ZeroMemory(Context->Buffer, sizeof(Context->Buffer));
		ZeroMemory(Context->BufferDS4, sizeof(Context->BufferDS4));
	}
}

EPollResult FWindowsDeviceInfo::PollTick(HANDLE Handle, unsigned char* Buffer, std::int32_t Length, DWORD& OutBytesRead)
{
	std::int32_t Err = ERROR_SUCCESS;
	PingOnce(Handle, &Err);

	OutBytesRead = 0;
	if (!ReadFile(Handle, Buffer, Length, &OutBytesRead, nullptr))
	{
		return EPollResult::Disconnected;
	}

	return EPollResult::ReadOk;
}

void FWindowsDeviceInfo::InitializeAudioDevice(FDeviceContext* Context)
{
	if (!Context || Context->ConnectionType != EDSDeviceConnection::Usb)
	{
		return;
	}

	GUID ControllerContainerID = GetHidDeviceContainerID(Context->Path);
	if (ControllerContainerID == GUID_NULL)
	{
		return;
	}

	IMMDeviceEnumerator* pEnumerator = nullptr;
	IMMDeviceCollection* pCollection = nullptr;

	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
	if (FAILED(hr))
	{
		return;
	}

	hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection);
	if (SUCCEEDED(hr))
	{
		UINT count;
		pCollection->GetCount(&count);
		for (UINT i = 0; i < count; i++)
		{
			IMMDevice* pDevice = nullptr;
			pCollection->Item(i, &pDevice);
			if (pDevice)
			{
				GUID AudioContainerID = GetAudioDeviceContainerID(pDevice);
				if (IsEqualGUID(ControllerContainerID, AudioContainerID))
				{
					IAudioClient* pAudioClient = nullptr;
					IAudioRenderClient* pRenderClient = nullptr;
					WAVEFORMATEX* pwfx = nullptr;

					if (SUCCEEDED(pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&pAudioClient)))
					{
						if (SUCCEEDED(pAudioClient->GetMixFormat(&pwfx)))
						{
							REFERENCE_TIME hnsRequestedDuration = 10000000;
							hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0, pwfx, nullptr);

							if (SUCCEEDED(hr))
							{
								hr = pAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&pRenderClient);
								if (SUCCEEDED(hr))
								{
									pAudioClient->Start();
									std::shared_ptr<FAudioDeviceContext> audioContext = Context->AudioContext;

									audioContext->Device = pDevice;
									audioContext->AudioClient = pAudioClient;
									audioContext->RenderClient = pRenderClient;
									audioContext->MixFormat = pwfx;
									audioContext->SampleRate = pwfx->nSamplesPerSec;
									audioContext->NumChannels = pwfx->nChannels;
									UE_LOG(LogDualSense, Log, TEXT("Successfully paired and initialized haptic audio for DualSense USB"));
								}
							}
						}
					}

					if (!Context->AudioContext->IsValid())
					{
						if (pwfx)
						{
							CoTaskMemFree(pwfx);
						}
						if (pRenderClient)
						{
							pRenderClient->Release();
						}
						if (pAudioClient)
						{
							pAudioClient->Release();
						}
						pDevice->Release();
					}

					break;
				}
				pDevice->Release();
			}
		}
		pCollection->Release();
	}
	pEnumerator->Release();
}

bool FWindowsDeviceInfo::PingOnce(HANDLE Handle, std::int32_t* OutLastError)
{
	FILE_STANDARD_INFO Info{};
	if (!GetFileInformationByHandleEx(Handle, FileStandardInfo, &Info, sizeof(Info)))
	{
		if (OutLastError)
		{
			*OutLastError = GetLastError();
		}
		return false;
	}
	if (OutLastError)
	{
		*OutLastError = ERROR_SUCCESS;
	}
	return true;
}

void FWindowsDeviceInfo::ProcessAudioHapitc(FDeviceContext* Context)
{
	if (!Context || !Context->Handle)
	{
		return;
	}

	if (Context->Handle == INVALID_PLATFORM_HANDLE)
	{
		return;
	}

	if (Context->ConnectionType != EDSDeviceConnection::Bluetooth)
	{
		return;
	}

	unsigned long BytesWritten = 0;
	constexpr size_t BufferSize = 142;
	if (!WriteFile(Context->Handle, Context->BufferAudio, BufferSize, &BytesWritten, nullptr))
	{
		const unsigned long Error = GetLastError();
		if (Error != ERROR_IO_PENDING)
		{
		}
	}
}

void FWindowsDeviceInfo::ConfigureFeatures(FDeviceContext* Context)
{
	unsigned char FeatureBuffer[41] = {0};
	std::memset(FeatureBuffer, 0, sizeof(FeatureBuffer));

	FeatureBuffer[0] = 0x05;
	if (!HidD_GetFeature(Context->Handle, FeatureBuffer, 41))
	{
		InvalidateHandle(Context);
		return;
	}

	using namespace FGamepadSensors;
	FGamepadCalibration Calibration;
	DualSenseCalibrationSensors(FeatureBuffer, Calibration);
	Context->Calibration = Calibration;
}
