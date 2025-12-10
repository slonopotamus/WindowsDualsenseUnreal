// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "Core/Types/Structs/Config/GamepadCalibration.h"

namespace FGamepadCalibrationSensors
{
	inline void DualSenseCalibrationSensors(const std::uint8_t* Buffer, FGamepadCalibration& OutCalibration)
	{
		auto GetLE16 = [](const uint8* Data) -> int16 {
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
		float DenomX = static_cast<float>(FMath::Abs(GyroPitchPlus - GyroPitchBias) + FMath::Abs(GyroPitchMinus - GyroPitchBias));
		OutCalibration.GyroFactorX = (DenomX != 0.0f) ? (Speed2x / DenomX) : 1.0f;

		float DenomY = static_cast<float>(FMath::Abs(GyroYawPlus - GyroYawBias) + FMath::Abs(GyroYawMinus - GyroYawBias));
		OutCalibration.GyroFactorY = (DenomY != 0.0f) ? (Speed2x / DenomY) : 1.0f;

		float DenomZ = static_cast<float>(FMath::Abs(GyroRollPlus - GyroRollBias) + FMath::Abs(GyroRollMinus - GyroRollBias));
		OutCalibration.GyroFactorZ = (DenomZ != 0.0f) ? (Speed2x / DenomZ) : 1.0f;

		// Acc X
		const float RangeX = static_cast<float>(AccelXPlus - AccelXMinus);
		OutCalibration.AccelBiasX = (AccelXPlus + AccelXMinus) / 2.0f;
		OutCalibration.AccelFactorX = (RangeX != 0.0f) ? (2.0f / RangeX) : 1.0f;

		// Acc Y
		const float RangeY = static_cast<float>(AccelYPlus - AccelYMinus);
		OutCalibration.AccelBiasY = (AccelYPlus + AccelYMinus) / 2.0f;
		OutCalibration.AccelFactorY = (RangeY != 0.0f) ? (2.0f / RangeY) : 1.0f;

		// Acc Z
		const float RangeZ = static_cast<float>(AccelZPlus - AccelZMinus);
		OutCalibration.AccelBiasZ = (AccelZPlus + AccelZMinus) / 2.0f;
		OutCalibration.AccelFactorZ = (RangeZ != 0.0f) ? (2.0f / RangeZ) : 1.0f;
	}

	inline void ProcessMotionData(const uint8* Buffer, const FGamepadCalibration& Calibration, DSCoreTypes::DSVector3D& FinalGyro, DSCoreTypes::DSVector3D & FinalAccel)
	{
		const std::int16_t RawGyroX = (Buffer[15] | (Buffer[16] << 8));
		const std::int16_t RawGyroY = (Buffer[17] | (Buffer[18] << 8));
		const std::int16_t RawGyroZ = (Buffer[19] | (Buffer[20] << 8));

		const int16 RawAccX = (Buffer[21] | (Buffer[22] << 8));
		const int16 RawAccY = (Buffer[23] | (Buffer[24] << 8));
		const int16 RawAccZ = (Buffer[25] | (Buffer[26] << 8));

		FinalGyro.X = ((RawGyroX - Calibration.GyroBiasX) * Calibration.GyroFactorX);
		FinalGyro.Y = ((RawGyroY - Calibration.GyroBiasY) * Calibration.GyroFactorY);
		FinalGyro.Z = ((RawGyroZ - Calibration.GyroBiasZ) * Calibration.GyroFactorZ);

		FinalAccel.X = ((RawAccX - Calibration.AccelBiasX) * Calibration.AccelFactorX);
		FinalAccel.Y = ((RawAccY - Calibration.AccelBiasY) * Calibration.AccelFactorY);
		FinalAccel.Z = ((RawAccZ - Calibration.AccelBiasZ) * Calibration.AccelFactorZ);
	}

} // namespace FGamepadCalibrationSensors
