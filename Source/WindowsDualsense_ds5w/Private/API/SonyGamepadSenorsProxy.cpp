// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "API/SonyGamepadSenorsProxy.h"
#include "API/SonyGamepadProxyHelpers.h"

using namespace SonyGamepadProxyHelpers;

void USonyGamepadSenorsProxy::StartMotionSensorCalibration(int32 ControllerId, float Duration, float DeadZone)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->StartMotionSensorCalibration(Duration, DeadZone);
	}
}
void USonyGamepadSenorsProxy::ResetGyroOrientation(int32 ControllerId)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->ResetGyroOrientation();
	}
}
bool USonyGamepadSenorsProxy::GetMotionSensorCalibrationStatus(int32 ControllerId, float& Progress)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return false;
	}
	return Gamepad->GetMotionSensorCalibrationStatus(Progress);
}
void USonyGamepadSenorsProxy::EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->EnableMotionSensor(bEnableGyroscope);
	}
}
