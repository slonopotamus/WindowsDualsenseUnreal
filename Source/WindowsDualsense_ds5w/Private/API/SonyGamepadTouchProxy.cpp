// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "API/SonyGamepadTouchProxy.h"
#include "API/SonyGamepadProxyHelpers.h"
#include "GCore/Interfaces/ISonyGamepad.h"

using namespace SonyGamepadProxyHelpers;
void USonyGamepadTouchProxy::EnableTouch(int32 ControllerId, bool bEnableTouch)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	return Gamepad->EnableTouch(bEnableTouch);
}

void USonyGamepadTouchProxy::EnableGesture(int32 ControllerId, bool bEnableGesture)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	return Gamepad->EnableGesture(bEnableGesture);
}
