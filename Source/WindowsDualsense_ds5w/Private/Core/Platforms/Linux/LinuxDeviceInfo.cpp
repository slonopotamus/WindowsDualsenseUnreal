// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "../../../../Public/Core/Platforms/Linux/LinuxDeviceInfo.h"
#include "hidapi_libusb.h"

static const uint16 SONY_VENDOR_ID = 0x054C;
static const uint16 DUALSHOCK4_PID_V1 = 0x05C4; // DualShock 4 (original model)
static const uint16 DUALSHOCK4_PID_V2 = 0x09CC; // DualShock 4 V2 (Slim/Pro)
static const uint16 DUALSENSE_PID     = 0x0CE6; // DualSense (PS5)
static const uint16 DUALSENSE_EDGE_PID= 0x0DF2; // DualSense Edge

void FLinuxDeviceInfo::Read(FDeviceContext* Context)
{
	if (!Context || !Context->Handle)
	{
		return;
	}

	int BytesRead = 0;
	
	if (Context->ConnectionType == Bluetooth && Context->DeviceType == EDeviceType::DualShock4)
	{
		const size_t InputReportLength = 547;
		BytesRead = hid_read(Context->Handle, Context->BufferDS4, InputReportLength);
		if (BytesRead < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to read from device (likely disconnected)"));
			InvalidateHandle(Context);
		}
		return;
	}
	
	const size_t InputReportLength = (Context->ConnectionType == Bluetooth) ? 78 : 64;
	if (sizeof(Context->Buffer) < InputReportLength)
	{
		UE_LOG(LogTemp, Error, TEXT("Buffer principal é muito pequeno para o relatório de input."));
		InvalidateHandle(Context);
		return;
	}

	BytesRead = hid_read(Context->Handle, Context->Buffer, InputReportLength);
	if (BytesRead < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to read from device (likely disconnected)"));
		InvalidateHandle(Context);
	}
}

void FLinuxDeviceInfo::Write(FDeviceContext* Context)
{
	if (!Context || !Context->Handle)
	{
		return;
	}

	const size_t InReportLength = (Context->DeviceType == EDeviceType::DualShock4) ? 32 : 74;
	const size_t OutputReportLength = (Context->ConnectionType == Bluetooth) ? 78 : InReportLength;

	int BytesWritten = hid_write(Context->Handle, Context->BufferOutput, OutputReportLength);
	if (BytesWritten < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to write to device"));
		InvalidateHandle(Context);
	}
}

void FLinuxDeviceInfo::Detect(TArray<FDeviceContext>& Devices)
{
	Devices.Empty();

	const TSet<uint16> SupportedPIDs = {
		DUALSHOCK4_PID_V1,
		DUALSHOCK4_PID_V2,
		DUALSENSE_PID,
		DUALSENSE_EDGE_PID
	};
	
	hid_device_info* Devs = hid_enumerate(SONY_VENDOR_ID, 0x0);
	if (!Devs)
	{
		UE_LOG(LogTemp, Log, TEXT("DualSense: Failed to enumerate devices."));
		return;
	}

	for (hid_device_info* CurrentDevice = Devs; CurrentDevice != nullptr; CurrentDevice = CurrentDevice->next)
	{
		if (SupportedPIDs.Contains(CurrentDevice->product_id))
		{
			FDeviceContext NewDeviceContext;
			FString PathStr = UTF8_TO_TCHAR(CurrentDevice->path);
			NewDeviceContext.Path = PathStr;
			
			switch (CurrentDevice->product_id)
			{
			case DUALSHOCK4_PID_V1:
			case DUALSHOCK4_PID_V2:
				NewDeviceContext.DeviceType = EDeviceType::DualShock4;
				break;
			case DUALSENSE_EDGE_PID:
				NewDeviceContext.DeviceType = EDeviceType::DualSenseEdge;
				break;
			case DUALSENSE_PID:
			default:
				NewDeviceContext.DeviceType = EDeviceType::DualSense;
				break;
			}

			NewDeviceContext.IsConnected = true;
			if (CurrentDevice->interface_number == -1 || CurrentDevice->bus_type == 5)
			{
				NewDeviceContext.ConnectionType = Bluetooth;
			}
			else
			{
				NewDeviceContext.ConnectionType = Usb;
			}
			NewDeviceContext.Handle = nullptr;
			Devices.Add(NewDeviceContext);
		}
	}
	hid_free_enumeration(Devs);
}

bool FLinuxDeviceInfo::CreateHandle(FDeviceContext* Context)
{
	if (!Context || Context->Path.IsEmpty())
	{
		return false;
	}
  
	const FTCHARToUTF8 PathConverter(*Context->Path);
	const FPlatformDeviceHandle Handle =  hid_open_path(PathConverter.Get());

	if (Handle == INVALID_PLATFORM_HANDLE)
	{
		UE_LOG(LogTemp, Error, TEXT("HIDManager: Failed to open device handle for the DualSense."));
		return false;
	}
	
	hid_set_nonblocking(Handle, 1);

	Context->Handle = Handle;
	return true;
}

void FLinuxDeviceInfo::InvalidateHandle(FDeviceContext* Context)
{
	if (Context && Context->Handle)
	{
		hid_close(Context->Handle);
		Context->Handle = INVALID_PLATFORM_HANDLE;
		Context->IsConnected = false;

		Context->Path = nullptr;
		memset(Context->Buffer, 0, sizeof(Context->Buffer));
		memset(Context->BufferDS4, 0, sizeof(Context->BufferDS4));
		memset(Context->BufferOutput, 0, sizeof(Context->BufferOutput));
	}
}
