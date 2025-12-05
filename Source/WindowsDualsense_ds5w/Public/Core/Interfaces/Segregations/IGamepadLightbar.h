// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "Core/Types/Enums/EDeviceCommons.h"

class WINDOWSDUALSENSE_DS5W_API IGamepadLightbar
{
public:
	virtual ~IGamepadLightbar() = default;
	/**
	 * Sets the lightbar color and associated timing parameters on the gamepad.
	 *
	 * @param Color The color to set the lightbar to, represented as an FColor.
	 * @param BrithnessTime The time duration for setting the brightness effect. Defaults to 0.0f.
	 * @param ToggleTime The time interval for toggling the lightbar. Defaults to 0.0f.
	 */
	virtual void SetLightbar(FColor Color, float BrithnessTime = 0.0f, float ToggleTime = 0.0f) = 0;
	/**
	 * Sets the LED associated with the player on the Sony gamepad to a specified brightness level.
	 *
	 * @param Led The LED associated with a specific player, represented as an ELedPlayerEnum.
	 * @param Brightness The desired brightness level for the LED, represented as an ELedBrightnessEnum.
	 */
	virtual void SetPlayerLed(ELedPlayerEnum Led, ELedBrightnessEnum Brightness) = 0;
	/**
	 * Sets the state of the microphone LED on the gamepad.
	 *
	 * @param Led The desired state of the microphone LED, represented by ELedMicEnum.
	 */
	virtual void SetMicrophoneLed(ELedMicEnum Led) = 0;
	/**
	 * Stops all currently active operations or actions associated with the interface.
	 * This method must be implemented by any derived class to handle the termination
	 * of all ongoing processes.
	 */
	virtual void ResetLights() = 0;
};
