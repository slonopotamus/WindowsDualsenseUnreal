// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "API/SonyGamepadSensorsProxy.h"
#include "API/SonyGamepadProxyHelpers.h"

using namespace SonyGamepadProxyHelpers;

void USonyGamepadSensorsProxy::ResetGyroOrientation(int32 ControllerId)
{
	if (auto* Gamepad = GetGamepad(ControllerId)->GetIGamepadSensors())
	{
		Gamepad->ResetGyroOrientation();
	}
}

void USonyGamepadSensorsProxy::EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope)
{
	if (auto* Gamepad = GetGamepad(ControllerId)->GetIGamepadSensors())
	{
		Gamepad->EnableMotionSensor(bEnableGyroscope);
	}
}
