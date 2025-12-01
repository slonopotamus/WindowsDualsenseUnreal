// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "CoreMinimal.h"

/**
 * Represents a collection of strength-related configurations and parameters for haptic feedback.
 * This struct is primarily used to define zones, timings, and adjustable parameters
 * for haptic effects on controller triggers.
 */
struct FGamepadTriggers
{
	unsigned char Compose[10] = {0};
};
