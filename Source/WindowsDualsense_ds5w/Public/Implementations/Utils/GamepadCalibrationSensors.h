// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "Core/Types/Structs/Config/GamepadCalibration.h"

namespace GamepadCalibrationSensors
{
	inline void DualSenseCalibrationSensors(const uint8* Buffer, FGamepadCalibration& OutCalibration)
	{
		auto GetLE16 = [](const uint8* Data) -> int16 {
			UE_LOG(LogDualSense, Log, TEXT("GetLE16: %d %d"), Data[0], Data[1]);
			return static_cast<int16>(Data[0] | (Data[1] << 8));
		};

		const int16 GyroPitchBias = GetLE16(&Buffer[1]);
		const int16 GyroYawBias = GetLE16(&Buffer[3]);
		const int16 GyroRollBias = GetLE16(&Buffer[5]);

		const int16 GyroPitchPlus = GetLE16(&Buffer[7]);
		const int16 GyroPitchMinus = GetLE16(&Buffer[9]);
		const int16 GyroYawPlus = GetLE16(&Buffer[11]);
		const int16 GyroYawMinus = GetLE16(&Buffer[13]);
		const int16 GyroRollPlus = GetLE16(&Buffer[15]);
		const int16 GyroRollMinus = GetLE16(&Buffer[17]);

		const int16 GyroSpeedPlus = GetLE16(&Buffer[19]);
		const int16 GyroSpeedMinus = GetLE16(&Buffer[21]);

		const int16 AccelXPlus = GetLE16(&Buffer[23]);
		const int16 AccelXMinus = GetLE16(&Buffer[25]);
		const int16 AccelYPlus = GetLE16(&Buffer[27]);
		const int16 AccelYMinus = GetLE16(&Buffer[29]);
		const int16 AccelZPlus = GetLE16(&Buffer[31]);
		const int16 AccelZMinus = GetLE16(&Buffer[33]);

		OutCalibration.GyroBiasX = GyroPitchBias;
		OutCalibration.GyroBiasY = GyroYawBias;
		OutCalibration.GyroBiasZ = GyroRollBias;

		const float Speed2x = static_cast<float>(GyroSpeedPlus + GyroSpeedMinus);
		float DenomX = static_cast<float>(abs(GyroPitchPlus - GyroPitchBias) + abs(GyroPitchMinus - GyroPitchBias));
		OutCalibration.GyroFactorX = (DenomX != 0.0f) ? (Speed2x / DenomX) : 1.0f;

		float DenomY = static_cast<float>(abs(GyroYawPlus - GyroYawBias) + abs(GyroYawMinus - GyroYawBias));
		OutCalibration.GyroFactorY = (DenomY != 0.0f) ? (Speed2x / DenomY) : 1.0f;

		float DenomZ = static_cast<float>(abs(GyroRollPlus - GyroRollBias) + abs(GyroRollMinus - GyroRollBias));
		OutCalibration.GyroFactorZ = (DenomZ != 0.0f) ? (Speed2x / DenomZ) : 1.0f;

		// Acc X
		const float RangeX = static_cast<float>(AccelXPlus - AccelXMinus);
		OutCalibration.AccelBiasX = AccelXPlus - RangeX / 2.0f;
		OutCalibration.AccelFactorX = 1.0f;

		// Acc Y
		float RangeY = static_cast<float>(AccelYPlus - AccelYMinus);
		OutCalibration.AccelBiasY = AccelYPlus - (RangeY / 2.0f);
		OutCalibration.AccelFactorY = 1.0f;

		// Acc Z
		float RangeZ = static_cast<float>(AccelZPlus - AccelZMinus);
		OutCalibration.AccelBiasZ = AccelZPlus - (RangeZ / 2.0f);
		OutCalibration.AccelFactorZ = 1.0f;

		UE_LOG(LogDualSense, Log, TEXT("AccelBias: %d  AccelFactorZ: %f"), OutCalibration.AccelBiasZ, OutCalibration.AccelFactorZ);
		UE_LOG(LogDualSense, Log, TEXT("AccelBiasY: %d GyroFactorZ: %f"), OutCalibration.GyroBiasZ, OutCalibration.GyroFactorZ);
	}

	inline void ProcessMotionData(const uint8* Buffer, const FGamepadCalibration& Calibration, FVector& FinalGyro, FVector& FinalAccel)
	{
		const int16 RawGyroX = static_cast<int16>(Buffer[16] | (Buffer[17] << 8));
		const int16 RawGyroY = static_cast<int16>(Buffer[18] | (Buffer[19] << 8));
		const int16 RawGyroZ = static_cast<int16>(Buffer[20] | (Buffer[21] << 8));

		const int16 RawAccX = static_cast<int16>(Buffer[22] | (Buffer[23] << 8));
		const int16 RawAccY = static_cast<int16>(Buffer[24] | (Buffer[25] << 8));
		const int16 RawAccZ = static_cast<int16>(Buffer[26] | (Buffer[27] << 8));

		FinalGyro.X = ((RawGyroX - Calibration.GyroBiasX) * Calibration.GyroFactorX);
		FinalGyro.Y = ((RawGyroY - Calibration.GyroBiasY) * Calibration.GyroFactorY);
		FinalGyro.Z = ((RawGyroZ - Calibration.GyroBiasZ) * Calibration.GyroFactorZ);

		FinalAccel.X = ((RawAccX - Calibration.AccelBiasX) * Calibration.AccelFactorX);
		FinalAccel.Y = ((RawAccY - Calibration.AccelBiasY) * Calibration.AccelFactorY);
		FinalAccel.Z = ((RawAccZ - Calibration.AccelBiasZ) * Calibration.AccelFactorZ);
	}

} // namespace GamepadCalibrationSensors
