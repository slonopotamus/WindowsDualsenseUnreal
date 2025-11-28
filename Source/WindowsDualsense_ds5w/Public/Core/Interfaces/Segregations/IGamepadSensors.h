// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"

class WINDOWSDUALSENSE_DS5W_API IGamepadSensors
{
public:
	virtual ~IGamepadSensors() = default;
	/**
	 * Resets the orientation of the gyroscope to its default state.
	 * Typically used to recalibrate the gyroscope sensor.
	 */
	virtual void ResetGyroOrientation() = 0;
	/**
	 * Enables the motion sensor functionality of the gamepad.
	 *
	 * @param bIsMotionSensor Specifies whether to enable the gyroscope (true) or accelerometer (false) as the motion sensor.
	 */
	virtual void EnableMotionSensor(bool bIsMotionSensor) = 0;
	/**
	 * Initiates the calibration process for the motion sensor on the gamepad.
	 *
	 * @param Duration The duration, in seconds, for which the calibration process should run.
	 * @param DeadZone The threshold value to be used for filtering out small movements.
	 */
	virtual void StartMotionSensorCalibration(float Duration, float DeadZone) = 0;
	/**
	 * Retrieves the current calibration status of the motion sensors.
	 *
	 * @param OutProgress A reference to a float where the current calibration progress will be stored.
	 *                    The value ranges from 0.0 (no progress) to 1.0 (fully calibrated).
	 * @return True if the calibration status was successfully retrieved, false otherwise.
	 */
	virtual bool GetMotionSensorCalibrationStatus(float& OutProgress) = 0;
};
