// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"

class WINDOWSDUALSENSE_DS5W_API IGamepadTouch
{
public:
	virtual ~IGamepadTouch() = default;
	/**
	 * Sets the touch state for the device.
	 *
	 * @param bIsTouch A boolean indicating whether touch input is enabled (true) or disabled (false).
	 */
	virtual void EnableTouch(const bool bIsTouch) = 0;
};
