// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "SonyGamepadProxy.h"
#include "Core/DeviceRegistry.h"
#include "Core/DualSense/DualSenseLibrary.h"
#include "Core/Interfaces/ISonyGamepad.h"
#include "Misc/CoreDelegates.h"

EDeviceType USonyGamepadProxy::GetDeviceType(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return EDeviceType::NotFound;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return EDeviceType::NotFound;
	}

	return Gamepad->GetDeviceType();
}

EDeviceConnection USonyGamepadProxy::GetConnectionType(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return EDeviceConnection::Unrecognized;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return EDeviceConnection::Unrecognized;
	}

	return Gamepad->GetConnectionType();
}

bool USonyGamepadProxy::DeviceIsConnected(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return false;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return false;
	}

	return true;
}

float USonyGamepadProxy::LevelBatteryDevice(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return 0.0f;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return 0.0f;
	}

	return Gamepad->GetBattery();
}

void USonyGamepadProxy::LedColorEffects(int32 ControllerId, FColor Color, float BrightnessTime, float ToogleTime)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetLightbar(Color, BrightnessTime, ToogleTime);
}

void USonyGamepadProxy::LedMicEffects(int32 ControllerId, ELedMicEnum Value)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetMicrophoneLed(Value);
}

void USonyGamepadProxy::StartMotionSensorCalibration(int32 ControllerId, float Duration, float DeadZone)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}
	Gamepad->StartMotionSensorCalibration(Duration, DeadZone);
}

void USonyGamepadProxy::ResetGyroOrientation(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}
	Gamepad->ResetGyroOrientation();
}

bool USonyGamepadProxy::GetMotionSensorCalibrationStatus(int32 ControllerId, float& Progress)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return false;
	}

	IGamepadSensors* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		UE_LOG(LogTemp, Error, TEXT("Gamepad not found"));
		return false;
	}

	return Gamepad->GetMotionSensorCalibrationStatus(Progress);
}

void USonyGamepadProxy::EnableTouch(int32 ControllerId, bool bEnableTouch)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->EnableTouch(bEnableTouch);
}

void USonyGamepadProxy::EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}
	Gamepad->EnableMotionSensor(bEnableGyroscope);
}

FInputDeviceId USonyGamepadProxy::GetGamepadInterface(int32 ControllerId)
{
	// We should never call into IPlatformInputDeviceMapper from non-game thread because it is not thread-safe
	check(IsInGameThread());

	TArray<FInputDeviceId> Devices;

	IPlatformInputDeviceMapper::Get().GetAllInputDevicesForUser(FPlatformUserId::CreateFromInternalId(ControllerId), Devices);

	for (const FInputDeviceId& DeviceId : Devices)
	{
		if (FDeviceRegistry::Get()->GetLibraryInstance(DeviceId))
		{
			return DeviceId;
		}
	}

	return FInputDeviceId::CreateFromInternalId(INDEX_NONE);
}
