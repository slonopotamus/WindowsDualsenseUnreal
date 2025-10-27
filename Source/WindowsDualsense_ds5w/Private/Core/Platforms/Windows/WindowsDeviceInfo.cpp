// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Core/Platforms/Windows/WindowsDeviceInfo.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"
#include <hidsdi.h>
#include <setupapi.h>

#include "Core/PlayStationOutputComposer.h"

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
		ZeroMemory(Context->BufferAudio, sizeof(Context->BufferAudio));
		ZeroMemory(Context->Buffer, sizeof(Context->Buffer));
	}
	//
	// if (Context->AudioHandle != INVALID_HANDLE_VALUE)
	// {
	// 	CloseHandle(Context->AudioHandle);
	// 	Context->AudioHandle = INVALID_HANDLE_VALUE;
	// }
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
	
	if (Context->ConnectionType != Bluetooth)
	{
		UE_LOG(LogTemp, Warning, TEXT("Audio haptics only supported over Bluetooth"));
		return;
	}
	
	constexpr size_t BufferSize = 276;
	DebugDumpAudioBuffer(Context->BufferAudio);
	DWORD BytesWritten = 0;
	if (!WriteFile(Context->Handle, Context->BufferAudio, BufferSize, &BytesWritten, nullptr))
	{
		const DWORD Error = GetLastError();
		if (Error != ERROR_IO_PENDING)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to send audio haptics via WriteFile. Error: %d"), Error);
		}

		UE_LOG(LogTemp, Error, TEXT("Failed to send audio haptics via WriteFile. Error: %d"), Error);
	}
}

bool FWindowsDeviceInfo::ConfigureBluetoothFeatures(HANDLE DeviceHandle)
{
	// Feature Report 0x05 - Enables advanced Bluetooth features
	// unsigned char FeatureBuffer[41];
	// FMemory::Memzero(FeatureBuffer, sizeof(FeatureBuffer));
	// FeatureBuffer[0] = 0x05;
	// if (!HidD_GetFeature(DeviceHandle, FeatureBuffer, 41))
	// {
	// 	const DWORD Error = GetLastError();
	// 	UE_LOG(LogTemp, Warning, TEXT("HIDManager: Failed to get Feature 0x05. Error: %d"), Error);
	// 	return false;
	// }

	// unsigned char OutputReport[272];
	// OutputReport[0] = 0x32;
	// OutputReport[1] = 0x00;
	// OutputReport[2] = 0b10010010;
	// OutputReport[3] = 0x08;
	// OutputReport[4] = 0x00;
	// OutputReport[5] = 0x00;
	// OutputReport[6] = 0x00;
	// OutputReport[7] = 0x00;
	// OutputReport[8] = 0x00;
	// OutputReport[9] = 0x00;
	// // Byte 1: valid_flag0
	// // Precisamos setar DS_OUTPUT_VALID_FLAG0_HAPTICS_SELECT (0x04)
	// // E DS_OUTPUT_VALID_FLAG0_COMPATIBLE_VIBRATION (0x01)
	//
	//
	// // Byte 45: valid_flag2
	// // Se você usar a v1 (acima), pode deixar 0x00.
	// // Se quisesse usar a v2, você setaria OutputReport[1] = 0x04
	// // e OutputReport[45] = 0x01 (DS_OUTPUT_VALID_FLAG2_COMPATIBLE_VIBRATION2)
	// const uint32 CrcChecksum = FPlayStationOutputComposer::Compute(OutputReport, 543);
	// OutputReport[0x4A] = static_cast<unsigned char>((CrcChecksum & 0x000000FF) >> 0UL);
	// OutputReport[0x4B] = static_cast<unsigned char>((CrcChecksum & 0x0000FF00) >> 8UL);
	// OutputReport[0x4C] = static_cast<unsigned char>((CrcChecksum & 0x00FF0000) >> 16UL);
	// OutputReport[0x4D] = static_cast<unsigned char>((CrcChecksum & 0xFF000000) >> 24UL);
	return true;
	

	// ... preencha o resto do report se necessário ...

	// Bytes 74-77: CRC32
	// Você DEVE calcular um CRC32 dos 74 bytes anteriores
	// e colocá-lo aqui se estiver usando o Report ID 0x31.
	// (Se você mudar o Report ID para 0x11, pode pular o CRC32)

	// Envie o report
	// if (!HidD_SetOutputReport(DeviceHandle, OutputReport, sizeof(OutputReport)))
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("HIDManager: Success to SET Feature 0x02"));
	// 	// ... lidar com erro ...
	// 	return false;
	// }
	// UE_LOG(LogTemp, Warning, TEXT("HIDManager: Success to SET Feature 0x02"));
	// return true;
}

void FWindowsDeviceInfo::DebugDumpAudioBuffer(unsigned char* AudioData)
{
	const int32 PacketPayloadSize = 272;
	UE_LOG(LogTemp, Warning, TEXT("========================================"));
	UE_LOG(LogTemp, Warning, TEXT("=== BUFFER DUMP BEFORE SENDING ==="));
	UE_LOG(LogTemp, Warning, TEXT("========================================"));

	UE_LOG(LogTemp, Warning, TEXT("--- BUFFER ---"));
	UE_LOG(LogTemp, Warning, TEXT("Actual Payload Size: %d bytes"), PacketPayloadSize);
	UE_LOG(LogTemp, Warning, TEXT("Total size from sizeof(): %llu bytes"), sizeof(AudioData)); // Apenas para depurar

	UE_LOG(LogTemp, Warning, TEXT("--- Header (Bytes 0-15) ---"));
	for (int i = 0; i < 16; i++)
	{
	    FString Note = TEXT("");
	    if (i == 2)  Note = TEXT(" <- Buffer ID (deve ser 0x92)");
	    if (i == 3)  Note = TEXT(" <- Sample Rate (deve ser 0x08)");
	    if (i == 10) Note = TEXT(" <- Sequence Counter");
	    if (i == 12) Note = TEXT(" <- Sample Count (deve ser 0x40)");
	    if (i == 15) Note = TEXT(" <- Fim do Header");
	    
	    UE_LOG(LogTemp, Warning, TEXT("0x%04X | 0x%02X | data[%d]%s"), 0x0004 + i, AudioData[i], i, *Note);
	}

	UE_LOG(LogTemp, Warning, TEXT("--- Audio Data (Bytes 16 a %d) ---"), PacketPayloadSize - 1);
	FString BufferString = TEXT("AudioData: ");
	for (int i = 16; i < PacketPayloadSize; i++)
	{
	    BufferString += FString::Printf(TEXT("0x%02X "), AudioData[i]);
	}

	UE_LOG(LogTemp, Warning, TEXT("--- Payload: \r \n--- %s"), *BufferString);

	// --- CRC ---
	// O CRC é (provavelmente) calculado e adicionado pela biblioteca HID (hidapi)
	// DEPOIS do seu payload de 272 bytes.
	UE_LOG(LogTemp, Warning, TEXT("--- CRC ---"));
	UE_LOG(LogTemp, Warning, TEXT("%02X,%02X,%02X,%02X "), AudioData[272], AudioData[273], AudioData[274], AudioData[275]);
	UE_LOG(LogTemp, Warning, TEXT("========================================"));
}
