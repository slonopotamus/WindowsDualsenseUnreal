// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

/**
 * Represents a collection of strength-related configurations and parameters for haptic feedback.
 * This struct is primarily used to define zones, timings, and adjustable parameters
 * for haptic effects on controller triggers.
 */
struct FGamepadTriggers
{
	std::uint8_t Compose[10] = {0};
};
