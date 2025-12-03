// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#define DS_ACC_RES_PER_G 8192
#define DS_ACC_RANGE (4 * DS_ACC_RES_PER_G)
#define DS_GYRO_RES_PER_DEG_S 1024
#define DS_GYRO_RANGE (2048 * DS_GYRO_RES_PER_DEG_S)
#define GRAVITY_MS2 9.80665f

/**
 * @brief Represents a structure for calibrating gamepad sensors.
 *
 * This structure is used to store bias and normalization parameters for gyroscopic and
 * accelerometer data retrieved from a gamepad. The values stored in this structure are
 * utilized to adjust raw sensor data for more accurate readings.
 *
 * Attributes:
 * - GyroBiasX, GyroBiasY, GyroBiasZ: Bias offsets for the gyroscope on the X, Y, and Z axes.
 * - AccelBiasX, AccelBiasY, AccelBiasZ: Bias offsets for the accelerometer on the X, Y, and Z axes.
 * - GyroFactorX, GyroFactorY, GyroFactorZ: Normalization factors for the gyroscope on the X, Y, and Z axes.
 * - AccelFactorX, AccelFactorY, AccelFactorZ: Normalization factors for the accelerometer on the X, Y, and Z axes.
 */
struct FGamepadCalibration
{
	int16 GyroBiasX = 0;
	int16 GyroBiasY = 0;
	int16 GyroBiasZ = 0;

	int16 AccelBiasX = 0;
	int16 AccelBiasY = 0;
	int16 AccelBiasZ = 0;

	float GyroFactorX = 1.0f;
	float GyroFactorY = 1.0f;
	float GyroFactorZ = 1.0f;

	float AccelFactorX = 1.0f;
	float AccelFactorY = 1.0f;
	float AccelFactorZ = 1.0f;
};
