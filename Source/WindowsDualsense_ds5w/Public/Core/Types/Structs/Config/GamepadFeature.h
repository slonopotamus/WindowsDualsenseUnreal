// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "CoreMinimal.h"

/**
 * Structure representing the configuration of features for a device.
 * This struct contains various settings related to vibration, rumble, and trigger behavior.
 */
struct FGamepadFeature
{
	/**
	 * Represents the mode of a feature in the FGamepadFeature struct.
	 *
	 * FeatureMode is used to configure a specific feature mode within the device.
	 * It is stored as an 8-bit unsigned integer, and its default value is 0xF7.
	 */
	uint8 FeatureMode = 0xF7;
	/**
	 * @brief Represents the mode or pattern of vibration for a device.
	 *
	 * This variable is used to define or control the vibration behavior,
	 * such as intensity, duration, or specific patterns. It can be used
	 * in various contexts like notifications, alerts, or feedback mechanisms
	 * in hardware or software systems.
	 *
	 * The exact usage and values that VibrationMode can hold may depend
	 * on the underlying implementation and hardware capabilities.
	 *
	 * Typical applications include:
	 * - Haptic feedback
	 * - Alert notifications
	 * - Game controller vibration patterns
	 *
	 * It is recommended to ensure that the vibration mode is compatible with
	 * the target hardware and does not cause unintended behavior or excessive
	 * power consumption.
	 */
	uint8 VibrationMode = 0xFF;
	/**
	 * @brief Adjusts and reduces the intensity of a soft rumble effect.
	 *
	 * The SoftRumbleReduce variable is used to control and diminish
	 * the intensity of a vibrational or rumbling feedback associated
	 * with a system or device. It provides a mechanism to finely tune
	 * or scale down the soft rumble effect to appropriate levels based
	 * on the desired application or user settings.
	 *
	 * Typical contexts for this variable include haptic feedback systems,
	 * gaming controllers, or any systems requiring adjustable rumble
	 * effects that necessitate a reduction in their intensity for user
	 * customization or system-specific requirements.
	 *
	 * This variable can be part of configurations to promote user
	 * comfort, power consumption management, or sensitivity adjustments.
	 */
	uint8 SoftRumbleReduce = 0x00;
	/**
	 * Represents the softness level for the trigger component of a game controller.
	 *
	 * The value determines the sensitivity or resistance level of the trigger mechanism.
	 * It is represented as an 8-bit unsigned integer, typically loaded or updated based on
	 * device settings or user customization.
	 *
	 * Default value: 0x4 (hexadecimal representation).
	 * Usage:
	 * - The `TriggerSoftnessLevel` is employed to configure or control the physical feedback
	 *   of the controller's trigger mechanism.
	 * - Often combined with other features such as vibration or rumble effects.
	 */
	uint8 TriggerSoftnessLevel = 0x00;
};
