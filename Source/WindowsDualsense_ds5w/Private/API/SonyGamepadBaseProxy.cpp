// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "API/SonyGamepadBaseProxy.h"
#include "API/SonyGamepadProxyHelpers.h"
#include "Core/Interfaces/ISonyGamepad.h"
#include "Core/Types/Enums/EDeviceConnection.h"
#include "Misc/CoreDelegates.h"

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
