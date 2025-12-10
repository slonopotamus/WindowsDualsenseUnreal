// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "Core/Types/DSCoreTypes.h"
using namespace DSCoreTypes;

class IGamepadRumbles
{
public:
	virtual ~IGamepadRumbles() = default;
	/**
	 * Sets the vibration levels for the gamepad.
	 *
	 * @param LeftRumble The intensity of the left-side vibration motor. The value should range from 0 (no vibration) to 255 (maximum intensity). Defaults to 0 if not specified.
	 * @param RightRumble The intensity of the right-side vibration motor. The value should range from 0 (no vibration) to 255 (maximum intensity). Defaults to 0 if not specified.
	 */
	virtual void SetVibration(std::uint8_t LeftRumble = 0, std::uint8_t RightRumble = 0) = 0;
};
