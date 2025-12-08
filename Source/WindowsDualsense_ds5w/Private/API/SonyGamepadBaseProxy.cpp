// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "API/SonyGamepadBaseProxy.h"
#include "API/SonyGamepadProxyHelpers.h"
#include "Core/Interfaces/ISonyGamepad.h"
#include "Core/Types/Enums/EDeviceConnection.h"
#include "Implementations/Libraries/DualSense/DualSenseLibrary.h"
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
void USonyGamepadBaseProxy::DualSenseSettings(int32 ControllerId, FDualSenseFeatureReport Value)
{
	FDualSenseLibrary* Gamepad = static_cast<FDualSenseLibrary*>(GetTriggerInterface(ControllerId));
	if (!Gamepad)
	{
		return;
	}
	Gamepad->Settings(Value);
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
void USonyGamepadBaseProxy::EnableTouch(int32 ControllerId, bool bEnableTouch)
{
	ISonyGamepad* Gamepad = GetGamepad(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	return Gamepad->EnableTouch(bEnableTouch);
}
