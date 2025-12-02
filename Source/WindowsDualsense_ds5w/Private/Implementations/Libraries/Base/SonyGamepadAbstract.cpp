// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Implementations/Libraries/Base/SonyGamepadAbstract.h"
#include "Core/Interfaces/IPlatformHardwareInfo.h"

void SonyGamepadAbstract::ShutdownLibrary()
{
	ButtonStates.Reset();
	IPlatformHardwareInfo::Get().InvalidateHandle(&HIDDeviceContexts);
}

void SonyGamepadAbstract::SetBatteryLevel(const float Value)
{
	if (Value > 100.f)
	{
		BatteryLevel = 100.f;
		return;
	}
	BatteryLevel = Value;
}

void SonyGamepadAbstract::CheckButtonInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, const FName ButtonName, const bool IsButtonPressed)
{
	const bool PreviousState = ButtonStates.Contains(ButtonName) ? ButtonStates[ButtonName] : false;
	if (IsButtonPressed && !PreviousState)
	{
		InMessageHandler.Get().OnControllerButtonPressed(ButtonName, UserId, InputDeviceId, false);
	}

	if (!IsButtonPressed && PreviousState)
	{
		InMessageHandler.Get().OnControllerButtonReleased(ButtonName, UserId, InputDeviceId, false);
	}

	ButtonStates.Add(ButtonName, IsButtonPressed);
}

void SonyGamepadAbstract::EnableTouch(const bool bIsTouch)
{
	bEnableTouch = bIsTouch;
}

void SonyGamepadAbstract::ResetGyroOrientation()
{
	bIsResetGyroscope = true;
}

void SonyGamepadAbstract::EnableMotionSensor(bool bIsMotionSensor)
{
	bEnableAccelerometerAndGyroscope = bIsMotionSensor;
}

void SonyGamepadAbstract::StartMotionSensorCalibration(float Duration, float DeadZone)
{
	SetIsCalibrating(true);
	SetCalibrationSampleCount(0);
	SetSensorsDeadZone(DeadZone);
	SetCalibrationDuration(Duration);
	SetGyroBaseline(FVector::ZeroVector);
	SetAccelBaseline(FVector::ZeroVector);
	SetAccumulatedGyro(FVector::ZeroVector);
	SetAccumulatedAccel(FVector::ZeroVector);
	SetCalibrationStartTime(FPlatformTime::Seconds());
}

bool SonyGamepadAbstract::GetMotionSensorCalibrationStatus(float& OutProgress)
{
	if (!bIsCalibrating)
	{
		OutProgress = 1.0f;
		return false;
	}

	const double ElapsedTime = FPlatformTime::Seconds() - CalibrationStartTime;
	OutProgress = FMath::Clamp(ElapsedTime / CalibrationDuration, 0.0, 1.0);

	if (ElapsedTime >= CalibrationDuration)
	{
		if (CalibrationSampleCount > 0)
		{
			GyroBaseline.X = AccumulatedGyro.X / CalibrationSampleCount;
			GyroBaseline.Y = AccumulatedGyro.Y / CalibrationSampleCount;
			GyroBaseline.Z = AccumulatedGyro.Z / CalibrationSampleCount;

			AccelBaseline.X = AccumulatedAccel.X / CalibrationSampleCount;
			AccelBaseline.Y = AccumulatedAccel.Y / CalibrationSampleCount;
			AccelBaseline.Z = AccumulatedAccel.Z / CalibrationSampleCount;
		}
		bIsCalibrating = false;
		bHasMotionSensorBaseline = true;
		return false;
	}

	return true;
}

float SonyGamepadAbstract::GetBattery() const
{
	return BatteryLevel;
}

bool SonyGamepadAbstract::IsConnected()
{
	return HIDDeviceContexts.IsConnected;
}

EDeviceType SonyGamepadAbstract::GetDeviceType()
{
	return HIDDeviceContexts.DeviceType;
}

EDeviceConnection SonyGamepadAbstract::GetConnectionType()
{
	return HIDDeviceContexts.ConnectionType;
}
