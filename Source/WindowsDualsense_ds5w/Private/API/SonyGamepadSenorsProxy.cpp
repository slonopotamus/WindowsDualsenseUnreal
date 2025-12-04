// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "API/SonyGamepadSenorsProxy.h"
#include "API/SonyGamepadProxyHelpers.h"

using namespace SonyGamepadProxyHelpers;

void USonyGamepadSenorsProxy::ResetGyroOrientation(int32 ControllerId)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->ResetGyroOrientation();
	}
}

void USonyGamepadSenorsProxy::EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->EnableMotionSensor(bEnableGyroscope);
	}
}
