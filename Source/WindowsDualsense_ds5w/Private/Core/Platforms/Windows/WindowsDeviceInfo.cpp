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
							if (!ConfigureBluetoothFeatures(TempDeviceHandle))
							{
								UE_LOG(LogTemp, Warning, TEXT("HIDManager: Failed to configure Bluetooth features."));
							}
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

bool FWindowsDeviceInfo::ConfigureBluetoothFeatures(HANDLE DeviceHandle)
{
	// Feature Report 0x05 - Enables advanced Bluetooth features
	unsigned char FeatureBuffer[41];
	FMemory::Memzero(FeatureBuffer, sizeof(FeatureBuffer));
	FeatureBuffer[0] = 0x05;
	if (!HidD_GetFeature(DeviceHandle, FeatureBuffer, 41))
	{
		const DWORD Error = GetLastError();
		UE_LOG(LogTemp, Warning, TEXT("HIDManager: Failed to get Feature 0x05. Error: %d"), Error);
		return false;
	}
	return true;
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

	// USB: Report 0x01 = 64 bytes
	// Bluetooth: Report 0x31 = 78 bytes (input)
	const size_t InputBufferSize = Context->ConnectionType == Bluetooth ? 78 : 64;
	HidD_FlushQueue(Context->Handle);
	DWORD BytesRead = 0;
	const EPollResult Response = PollTick(Context->Handle, Context->Buffer, InputBufferSize, BytesRead);
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
	size_t OutputReportLength = Context->ConnectionType == Bluetooth ? 78 : InReportLength;

	// Bluetooth DualSense:
	// Report 0x31 (78 bytes) - Triggers, LEDs, Rumble
	DWORD BytesWritten = 0;
	if (!WriteFile(Context->Handle, Context->BufferOutput, OutputReportLength, &BytesWritten, nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to write output report 0x02/0x31 data to device. report %llu error Code: %d"),
			OutputReportLength, GetLastError());
		InvalidateHandle(Context);
	}
}

bool FWindowsDeviceInfo::CreateHandle(FDeviceContext* DeviceContext)
{
	if (DeviceContext->Handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(DeviceContext->Handle);
		DeviceContext->Handle = INVALID_HANDLE_VALUE;
	}

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

		ZeroMemory(Context->BufferOutput, sizeof(Context->BufferOutput));
		ZeroMemory(Context->BufferAudio.Raw, sizeof(Context->BufferAudio.Raw));
		ZeroMemory(Context->Buffer, sizeof(Context->Buffer));
	}

	if (Context->AudioHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(Context->AudioHandle);
		Context->AudioHandle = INVALID_HANDLE_VALUE;
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

void FWindowsDeviceInfo::ProcessAudioHapitc(FDeviceContext* Context)
{
	if (!Context || !Context->Handle)
	{
		UE_LOG(LogTemp, Error, TEXT("Context not found!"));
		return;
	}

	if (Context->Handle == INVALID_HANDLE_VALUE)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid device handle before attempting to read"));
		return;
	}
	//
	// if (Context->ConnectionType != Bluetooth)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Audio haptics only supported over Bluetooth"));
	// 	return;
	// }
	//
	constexpr size_t BufferSize = 142;
	DebugDumpAudioBuffer(Context->BufferAudio);
	
	DWORD BytesWritten = 0;
	if (!WriteFile(Context->Handle, Context->BufferAudio.Raw, BufferSize, &BytesWritten, nullptr))
	{
		const DWORD Error = GetLastError();
		if (Error != ERROR_IO_PENDING)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to send audio haptics via WriteFile. Error: %d"), Error);
		}
	}
}

void FWindowsDeviceInfo::DebugDumpAudioBuffer(const FDualSenseHapticBuffer& AudioData)
{
	UE_LOG(LogTemp, Warning, TEXT("========================================"));
	UE_LOG(LogTemp, Warning, TEXT("=== BUFFER DUMP BEFORE SENDING ==="));
	UE_LOG(LogTemp, Warning, TEXT("========================================"));
	
	UE_LOG(LogTemp, Warning, TEXT("--- BUFFER ---"));
	UE_LOG(LogTemp, Warning, TEXT("Total size: 142 bytes"));
	UE_LOG(LogTemp, Warning, TEXT("Payload size: 137 bytes"));
	UE_LOG(LogTemp, Warning, TEXT("CRC size: 4 bytes"));
	UE_LOG(LogTemp, Warning, TEXT(""));

	// Header
	UE_LOG(LogTemp, Warning, TEXT("Offset | Byte | Description"));
	UE_LOG(LogTemp, Warning, TEXT("-------|------|------------------------------------------"));
	UE_LOG(LogTemp, Warning, TEXT("0x0000 | 0x%02X | Report ID (expected: 0x32)"), AudioData.Report.Header.Report_ID);
	UE_LOG(LogTemp, Warning, TEXT("0x0001 | 0x%02X | Tag=0x%X, Seq=0x%X"), 
		AudioData.Report.Header.Tag_Seq,
		(AudioData.Report.Header.Tag_Seq >> 4) & 0x0F,
		AudioData.Report.Header.Tag_Seq & 0x0F);
	
	// Packet 0x11
	UE_LOG(LogTemp, Warning, TEXT(""));
	UE_LOG(LogTemp, Warning, TEXT("--- Packet 0x11 (offset 0x0002) ---"));
	UE_LOG(LogTemp, Warning, TEXT("0x0002 | 0x%02X | pid=0x%02X, unk=%d, sized=%d (expected: 0x91)"), 
		AudioData.Report.Pkt11.pid,
		AudioData.Report.Pkt11.pid & 0x7F,
		(AudioData.Report.Pkt11.unk >> 0x7) & 1,       // Bit 7 (MSB de um uint8)
		(AudioData.Report.Pkt11.sized >> 0x7) & 1);
	UE_LOG(LogTemp, Warning, TEXT("0x0003 | 0x%02X | length=%d (expected: 7)"), 
		AudioData.Report.Pkt11.length, AudioData.Report.Pkt11.length);
	
	for (int i = 0; i < 7; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("0x%04X | 0x%02X | data[%d]%s"), 
			0x0004 + i, 
			AudioData.Report.Pkt11.data[i], 
			i,
			i == 6 ? TEXT(" <- counter (ii)") : TEXT(""));
	}
	
	// Packet 0x12
	UE_LOG(LogTemp, Warning, TEXT(""));
	UE_LOG(LogTemp, Warning, TEXT("--- Packet 0x12 (offset 0x000B) ---"));
	UE_LOG(LogTemp, Warning, TEXT("0x000B | 0x%02X | pid=0x%02X, unk=%d, sized=%d (expected: 0x92)"), 
		AudioData.Report.Pkt12.pid,
		AudioData.Report.Pkt12.pid & 0x7F,
		(AudioData.Report.Pkt12.unk >> 0x7) & 1,       // Bit 7 (MSB de um uint8)
		(AudioData.Report.Pkt12.sized >> 0x7) & 1); 
	UE_LOG(LogTemp, Warning, TEXT("0x000C | 0x%02X | length=%d (expected: 64)"), 
		AudioData.Report.Pkt12.length, AudioData.Report.Pkt12.length);
	
	// Audio samples (64 bytes) 
	UE_LOG(LogTemp, Warning, TEXT(""));
	UE_LOG(LogTemp, Warning, TEXT("--- Audio samples (64 bytes) ---"));
	for (int line = 0; line < 4; line++)
	{
		FString HexLine;
		for (int col = 0; col < 16; col++)
		{
			int idx = line * 16 + col;
			HexLine += FString::Printf(TEXT("%02X "), AudioData.Report.Pkt12.data[idx]);
		}
		UE_LOG(LogTemp, Warning, TEXT("0x%04X | %s"), 0x000D + (line * 16), *HexLine);
	}
	
	// Padding (61 bytes)
	UE_LOG(LogTemp, Warning, TEXT(""));
	UE_LOG(LogTemp, Warning, TEXT("--- Padding (61 bytes) ---"));
	int paddingStart = 0x004D;
	for (int line = 0; line < 4; line++)
	{
		FString HexLine;
		int bytesToShow = (line == 3) ? 13 : 16; // Última linha tem apenas 13 bytes
		for (int col = 0; col < bytesToShow; col++)
		{
			int idx = line * 16 + col;
			if (idx < 61)
			{
				HexLine += FString::Printf(TEXT("%02X "), AudioData.Raw[idx]);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("0x%04X | %s"), paddingStart + (line * 16), *HexLine);
	}
	
	// CRC (4 bytes)
	UE_LOG(LogTemp, Warning, TEXT(""));
	UE_LOG(LogTemp, Warning, TEXT("--- CRC (4 bytes at offset 0x008A) ---"));
	UE_LOG(LogTemp, Warning, TEXT("0x008A | %02X %02X %02X %02X"), 
		AudioData.Raw[138],
		AudioData.Raw[139],
		AudioData.Raw[140],
		AudioData.Raw[141]);
	
	UE_LOG(LogTemp, Warning, TEXT(""));
	UE_LOG(LogTemp, Warning, TEXT("========================================"));
}
