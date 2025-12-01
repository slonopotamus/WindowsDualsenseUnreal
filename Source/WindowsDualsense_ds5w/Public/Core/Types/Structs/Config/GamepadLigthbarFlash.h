// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"

/**
 * Represents the player LED configuration for a device.
 */
struct FGamepadLigthbarFlash
{
	unsigned char Bright_Time = 0x0;
	unsigned char Toggle_Time = 0x0;
};
