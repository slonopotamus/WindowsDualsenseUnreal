// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "CoreMinimal.h"

/**
 * @class FGamepadRumbles
 *
 * @brief The FRumbles class is designed to manage and control rumble feedback for devices such as game controllers.
 *
 * This class provides functionalities to initialize, update, and manipulate rumble effects
 * as part of a feedback mechanism. It is intended to enhance user interaction by simulating
 * vibrations or force feedback based on specific events or triggers.
 *
 * The FRumbles class encapsulates all the necessary operations required to handle the lifecycle
 * of a rumble instance, providing methods for configuring rumble patterns, managing their intensity,
 * and activation state.
 */
struct FGamepadRumbles
{
	/**
	 * @brief Represents the left-hand side position or value in a given context.
	 *
	 * The `Left` variable is often used to denote a position, direction, or value
	 * that is associated with or located at the left-side in a specific application
	 * or system. Its exact usage and meaning may vary depending on the context
	 * within which it is used.
	 *
	 * @note Ensure that the context or system in which `Left` is applied
	 * defines its behavior and associated interactions clearly.
	 */
	unsigned char Left = 0x0;
	/**
	 * @brief Represents the right direction or orientation in a spatial or logical context.
	 *
	 * The variable `Right` is typically used to signify the direction that is opposite to left.
	 * It can be applied in various domains such as geometry, navigation, or logical conditions
	 * where operations or processes involve orientation, movement, or side specification.
	 */
	unsigned char Right = 0x0;
};
