// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Gamepad/SonyGamepadBaseProxy.h"
#include "Core/DualSense/DualSenseLibrary.h"
#include "Core/Interfaces/ISonyGamepad.h"
#include "Gamepad/SonyGamepadProxyHelpers.h"
#include "Misc/CoreDelegates.h"

DEFINE_LOG_CATEGORY(LogDualSense);
using namespace SonyGamepadProxyHelpers;

EDeviceType USonyGamepadBaseProxy::GetDeviceType(int32 ControllerId)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return EDeviceType::NotFound;
	}

	return Gamepad->GetDeviceType();
}
EDeviceConnection USonyGamepadBaseProxy::GetConnectionType(int32 ControllerId)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return EDeviceConnection::Unrecognized;
	}

	return Gamepad->GetConnectionType();
}
bool USonyGamepadBaseProxy::DeviceIsConnected(int32 ControllerId)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return false;
	}

	return true;
}
float USonyGamepadBaseProxy::LevelBatteryDevice(int32 ControllerId)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return 0.0f;
	}

	return Gamepad->GetBattery();
}
void USonyGamepadBaseProxy::LedColorEffects(int32 ControllerId, FColor Color, float BrightnessTime, float ToogleTime)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetLightbar(Color, BrightnessTime, ToogleTime);
}
void USonyGamepadBaseProxy::LedMicEffects(int32 ControllerId, ELedMicEnum Value)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetMicrophoneLed(Value);
}
void USonyGamepadBaseProxy::StartMotionSensorCalibration(int32 ControllerId, float Duration, float DeadZone)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return;
	}
	Gamepad->StartMotionSensorCalibration(Duration, DeadZone);
}
void USonyGamepadBaseProxy::ResetGyroOrientation(int32 ControllerId)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return;
	}
	Gamepad->ResetGyroOrientation();
}
bool USonyGamepadBaseProxy::GetMotionSensorCalibrationStatus(int32 ControllerId, float& Progress)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return false;
	}
	return Gamepad->GetMotionSensorCalibrationStatus(Progress);
}
void USonyGamepadBaseProxy::EnableTouch(int32 ControllerId, bool bEnableTouch)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return;
	}
	Gamepad->EnableTouch(bEnableTouch);
}
void USonyGamepadBaseProxy::EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return;
	}
	Gamepad->EnableMotionSensor(bEnableGyroscope);
}
