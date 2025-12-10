// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

/**
 * Represents the player LED configuration for a device.
 */
struct FGamepadLigthbarFlash
{
	std::uint8_t Bright_Time = 0x0;
	std::uint8_t Toggle_Time = 0x0;
};
