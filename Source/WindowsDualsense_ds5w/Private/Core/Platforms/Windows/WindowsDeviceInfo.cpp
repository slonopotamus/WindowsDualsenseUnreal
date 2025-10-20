// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025




#include "Core/Platforms/Windows/WindowsDeviceInfo.h"

#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"

#include <hidsdi.h>
#include <setupapi.h>

void FWindowsDeviceInfo::Detect(TArray<FDeviceContext>& Devices)
{
	GUID HidGuid;
	HidD_GetHidGuid(&HidGuid);

	const HDEVINFO DeviceInfoSet = SetupDiGetClassDevs(&HidGuid, nullptr, nullptr,
	                                                   DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (DeviceInfoSet == INVALID_HANDLE_VALUE)
	{
		UE_LOG(LogTemp, Error, TEXT("HIDManager: Falha ao obter informações dos dispositivos HID."));
		return;
	}

	SP_DEVICE_INTERFACE_DATA DeviceInterfaceData = {};
	DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	TMap<int32, FString> DevicePaths;
	DevicePaths.Empty();
	for (int32 DeviceIndex = 0; SetupDiEnumDeviceInterfaces(DeviceInfoSet, nullptr, &HidGuid, DeviceIndex,
	                                                        &DeviceInterfaceData); DeviceIndex++)
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
			const HANDLE TempDeviceHandle = CreateFileW(
				DetailDataBuffer->DevicePath,
				GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, NULL, nullptr
			);
			
			if (
				TempDeviceHandle != INVALID_HANDLE_VALUE
			)
			{
				HIDD_ATTRIBUTES Attributes = {};
				Attributes.Size = sizeof(HIDD_ATTRIBUTES);

				if (HidD_GetAttributes(TempDeviceHandle, &Attributes))
				{
					if (
						Attributes.VendorID == 0x054C &&
						(
							Attributes.ProductID == 0x0CE6 ||
							Attributes.ProductID == 0x0DF2 ||
							Attributes.ProductID == 0x05C4 ||
							Attributes.ProductID == 0x09CC
						)
					)
					{
						FDeviceContext Context = {};
						WCHAR DeviceProductString[260];
						if (!HidD_GetProductString(TempDeviceHandle, DeviceProductString, 260))
						{
							UE_LOG(LogTemp, Error, TEXT("HIDManager: Failed to obtain device path for the DualSense."));
							continue;
						}

						if (DevicePaths.Contains(DeviceIndex))
						{
							continue;
						}

						FString PathStr(DetailDataBuffer->DevicePath);
						Context.Path = PathStr;
						DevicePaths.Add(DeviceIndex, PathStr);
						switch (Attributes.ProductID)
						{
							case 0x05C4:
							case 0x09CC:
								Context.DeviceType = DualShock4;
								break;
							case 0x0DF2:
								Context.DeviceType = DualSenseEdge;
								break;
							default: Context.DeviceType = DualSense;
						}
						
						Context.IsConnected = true;
						Context.ConnectionType = Usb;
						Context.Handle = TempDeviceHandle;
						FString DevicePath(Context.Path);
						if (DevicePath.Contains(TEXT("{00001124-0000-1000-8000-00805f9b34fb}")) ||
							DevicePath.Contains(TEXT("bth")) ||
							DevicePath.Contains(TEXT("BTHENUM")))
						{
							Context.ConnectionType = Bluetooth;
						}
						Devices.Add(Context);
					}
				}
				CloseHandle(TempDeviceHandle);
			}
		}
		free(DetailDataBuffer);
	}

	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
}

void FWindowsDeviceInfo::ProcessAudioHapitc(FDeviceContext* Context, FDualSenseHapictBuffer* AudioData)
{
	if (!Context || !Context->Handle)
	{
		UE_LOG(LogTemp, Error, TEXT("Context not found!"));
		return;
	}

	
	
	DWORD BytesWritten = 0;
	if (!WriteFile(Context->Handle, &AudioData->report, sizeof(AudioData->report), &BytesWritten, nullptr))
	{
		DWORD Error = GetLastError();
		if (Error == ERROR_INVALID_PARAMETER || Error == ERROR_IO_PENDING)
		{
			UE_LOG(LogTemp, Warning, TEXT("DualSense write temporary error (expected for BT). Error: %d"), Error);
			return;
		}
    
		UE_LOG(LogTemp, Error, TEXT("Failed DualSense write. Error: %d"), Error);
		InvalidateHandle(Context);
		return;
	}

	FString Line;
	Line += TEXT("\nPKT12 AFTER WRITE: ");
	Line += FString::Printf(TEXT("tag:%02X prefix:%02X id:%02X size:%02X "), 
		AudioData->report.pkt12.tag,
		AudioData->report.pkt12.prefix,
		AudioData->report.pkt12.id,
		AudioData->report.pkt12.size);

	Line += TEXT("data: ");
	for (int32 i = 0; i < sizeof(AudioData->report.pkt12.data); ++i)
	{
		Line += FString::Printf(TEXT("%02X "), AudioData->report.pkt12.data[i]);
	}

	UE_LOG(LogTemp, Warning, TEXT("Line: %s"), *Line);

	size_t InputReportLength = sizeof(AudioData->raw);
	UE_LOG(LogTemp, Log, TEXT("DualSense Audio Buffer [%llu bytes]:\n%s"), 
		InputReportLength, *Line);

	if (BytesWritten < sizeof(AudioData->raw))
	{
		UE_LOG(LogTemp, Warning, TEXT("Partial write to DualSense (%lu of %llu bytes)"), 
			BytesWritten, sizeof(AudioData->raw));
	}
	
	uint8_t responseBuffer[142];
	DWORD BytesRead = 0;

	if (!ReadFile(Context->Handle, 
				  responseBuffer,
				  sizeof(responseBuffer),
				  &BytesRead,
				  nullptr))
	{
		DWORD Error = GetLastError();
		if (Error == ERROR_INVALID_PARAMETER || Error == ERROR_IO_PENDING)
		{
			UE_LOG(LogTemp, Warning, TEXT("DualSense read temporary error (expected for BT). Error: %d"), Error);
			return;
		}
   
		UE_LOG(LogTemp, Error, TEXT("Failed DualSense read. Error: %d"), Error);
	}

	// Se a leitura foi bem sucedida, copiamos os dados para a estrutura
	if (BytesRead == sizeof(responseBuffer))
	{
		FMemory::Memcpy(&AudioData->raw, responseBuffer, sizeof(responseBuffer));
	}

}

void FWindowsDeviceInfo::Read(FDeviceContext* Context)
{
	if (!Context)
	{
		UE_LOG(LogTemp, Error, TEXT("Context nto found!"));
		return;
	}
	
	if (Context->Handle == INVALID_HANDLE_VALUE)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid device handle before attempting to read"));
		return;
	}
	
	if (!Context->IsConnected)
	{
		InvalidateHandle(Context);
		UE_LOG(LogTemp, Error, TEXT("Dualsense: DeviceContext->Connected, false"));
		return;
	}
	Context->Buffer.ExpandForAudio(547);
	
	HidD_FlushQueue(Context->Handle);
	DWORD BytesRead = 0;
	const EPollResult Response = PollTick(Context->Handle, Context->Buffer.GetData(), Context->Buffer.GetSize(), BytesRead);
	if (Response == EPollResult::Disconnected)
	{
		InvalidateHandle(Context);
	}
}

void FWindowsDeviceInfo::Write(FDeviceContext* Context)
{
	if (Context->Handle == INVALID_HANDLE_VALUE)
	{
		return;
	}
	
	size_t InReportLength = Context->DeviceType == DualShock4 ? 32 : 74;
	size_t OutputReportLength = Context->ConnectionType == Bluetooth ? 142 : InReportLength;

	DWORD BytesWritten = 0;
	if (!WriteFile(Context->Handle, Context->BufferOutput, OutputReportLength, &BytesWritten, nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to write output report 0x02 data to device. report %llu error Code: %d"),
			OutputReportLength, GetLastError());
		InvalidateHandle(Context);
	}
}

bool FWindowsDeviceInfo::CreateHandle(FDeviceContext* DeviceContext)
{
	const HANDLE DeviceHandle = CreateFileW(
			*DeviceContext->Path,
			GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, NULL, nullptr
		);

	if (DeviceHandle == INVALID_HANDLE_VALUE)
	{
		DeviceContext->Handle = DeviceHandle;
		UE_LOG(LogTemp, Error, TEXT("HIDManager: Failed to open device handle for the DualSense."));
		return false;
	}
	
	DeviceContext->Handle = DeviceHandle;
	return true;
}

void FWindowsDeviceInfo::InvalidateHandle(FDeviceContext* Context)
{
	IPlatformInputDeviceMapper::Get().Internal_SetInputDeviceConnectionState(Context->UniqueInputDeviceId, EInputDeviceConnectionState::Disconnected);
	if (!Context)
	{
		return;
	}
	
	if (Context->Handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(Context->Handle);
		Context->Handle = INVALID_HANDLE_VALUE;
		Context->IsConnected = false;
		Context->Path = nullptr;

		Context->Buffer.ResetToNormal();
		ZeroMemory(Context->BufferOutput, sizeof(Context->BufferOutput));
		ZeroMemory(Context->BufferAudio, sizeof(Context->BufferAudio));
		UE_LOG(LogTemp, Log, TEXT("HIDManager: Invalidate Handle."));
	}
}

void FWindowsDeviceInfo::InvalidateHandle(HANDLE Handle)
{
	if (Handle != INVALID_PLATFORM_HANDLE)
	{
		CloseHandle(Handle);
		UE_LOG(LogTemp, Log, TEXT("HIDManager: Invalidate Handle."));
	}
}

EPollResult FWindowsDeviceInfo::PollTick(HANDLE Handle, unsigned char* Buffer, int32 Length, DWORD& OutBytesRead)
{
	int32 Err = ERROR_SUCCESS;
	PingOnce(Handle, &Err);
	
	OutBytesRead = 0;
	if (!ReadFile(Handle, Buffer, Length, &OutBytesRead, nullptr))
	{
		const int32 Error = GetLastError();
		if (ShouldTreatAsDisconnected(Error))
		{
			return EPollResult::Disconnected;
		}

		InvalidateHandle(Handle);
	}

	return EPollResult::ReadOk;
}


bool FWindowsDeviceInfo::PingOnce(HANDLE Handle, int32* OutLastError)
{
	FILE_STANDARD_INFO Info{};
	if (!GetFileInformationByHandleEx(Handle, FileStandardInfo, &Info, sizeof(Info)))
	{
		if (OutLastError) *OutLastError = GetLastError();
		return false;
	}
	if (OutLastError) *OutLastError = ERROR_SUCCESS;
	return true;
}
