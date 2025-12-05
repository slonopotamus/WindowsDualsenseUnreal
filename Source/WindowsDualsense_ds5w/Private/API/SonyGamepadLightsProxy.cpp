// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "API/SonyGamepadLightsProxy.h"
#include "API/SonyGamepadProxyHelpers.h"

using namespace SonyGamepadProxyHelpers;

void USonyGamepadLightsProxy::Ligthbar(int32 ControllerId, FColor Color)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->SetLightbar(Color);
	}
}

void USonyGamepadLightsProxy::LigthbarFlash(int32 ControllerId, FColor Color, float BrithnessTime, float ToggleTime)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->SetLightbar(Color, BrithnessTime, ToggleTime);
	}
}

void USonyGamepadLightsProxy::PlayerLed(int32 ControllerId, ELedPlayerEnum Value, ELedBrightnessEnum Brightness)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->SetPlayerLed(Value, Brightness);
	}
}
