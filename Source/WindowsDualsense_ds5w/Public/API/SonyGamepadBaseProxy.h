// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "Core/Types/Enums/EDeviceCommons.h"
#include "Core/Types/Enums/EDeviceConnection.h"
#include "Core/Interfaces/ISonyGamepad.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"
#if PLATFORM_WINDOWS
#include "Windows/WindowsApplication.h"
#endif
#include "SonyGamepadBaseProxy.generated.h"

/**
 * Proxy class for interacting with Sony gamepad devices such as DualSense or DualShock controllers.
 * This class provides static methods to handle various functionalities, including device status,
 * connection type, battery levels, LED effects, motion sensor calibration, touch capabilities,
 * and deprecation of certain methods.
 */
UCLASS(Blueprintable, BlueprintType)
class WINDOWSDUALSENSE_DS5W_API USonyGamepadBaseProxy : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Checks if the DualSense or DualShock device with the specified Controller ID is connected.
	 *
	 * @param ControllerId The ID of the controller to check for connectivity.
	 * @return True if the DualSense or DualShock  device is connected, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad Status", meta = (DisplayName = "Device Is Connected", ToolTip = "Checks if the device with the specified Controller ID is connected."))
	static bool DeviceIsConnected(int32 ControllerId);
	/**
	 * Retrieves the device type of a connected gamepad based on the provided controller ID.
	 *
	 * @param ControllerId The ID of the controller to query for its device type.
	 * @return The type of the device connected (e.g., DualSense, DualShock4, or NotFound) as an EDeviceType.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad Status", meta = (DisplayName = "Device Type (e.g., DualSense)", ToolTip = "The type of the device connected (e.g., DualSense, DualShock4, or NotFound) as an EDeviceType."))
	static EDeviceType GetDeviceType(int32 ControllerId);
	/**
	 * Retrieves the connection type of the DualSense or DualShock device with the specified Controller ID.
	 *
	 * @param ControllerId The ID of the controller whose connection type is being queried.
	 * @return The connection type of the device, which can be USB, Bluetooth, or Unrecognized.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad Status", meta = (DisplayName = "Connection Type (USB, Bluetooth, Unknown)"))
	static EDeviceConnection GetConnectionType(int32 ControllerId);
	/**
	 * Retrieves the battery level of the DualSense or DualShock controller for the specified controller ID.
	 *
	 * This method uses the DualSense or DualShock library instance associated with the provided controller ID
	 * to fetch the battery level. If the library instance is not found, the method returns 0.0f.
	 *
	 * @param ControllerId The ID of the DualSense or DualShock controller to query.
	 * @return The battery level of the controller as a float. Returns 0.0f if the library instance cannot be retrieved.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad Status", meta = (DisplayName = "Battery Level (0.0f-100.0f)"))
	static float LevelBatteryDevice(int32 ControllerId);
	/**
	 * Enables or disables the touch functionality on a specified DualSense controller.
	 *
	 * @param ControllerId The identifier of the controller for which the touch functionality should be enabled or disabled.
	 * @param bEnableTouch A boolean indicating whether to enable (true) or disable (false) the touch functionality.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad: Dualsense or DualShock Touch (Legacy)", meta = (AdvancedDisplay = "EnableTouch Deprecated v1.2.20"))
	static void EnableTouch(int32 ControllerId, bool bEnableTouch);
	/**
	 * Updates the LED color effects on a DualSense controller using the specified color.
	 *
	 * @param ControllerId The identifier of the controller whose LED color will be updated.
	 * @param Color The color to set on the controller's LED.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad (Legacy)", meta = (AdvancedDisplay = "LedMicEffects Deprecated v1.2.20"))
	static void LedColorEffects(
	    int32 ControllerId,
	    FColor Color,
	    UPARAM(DisplayName = "(DualShock 4) transition time max: 2.5f", meta = (ClampMin = "0.0", ClampMax = "2.5", UIMin = "0.0", UIMax = "2.5", ToolTip = "(DualShock) LED brightness transition time, in seconds."))
	        const float BrightnessTime = 0.0f,
	    UPARAM(DisplayName = "(DualShock 4) Toggle transition max: 2.5f", meta = (ClampMin = "0.0", ClampMax = "2.5", UIMin = "0.0", UIMax = "2.5", ToolTip = "(DualShock) Toggle transition time, in seconds."))
	        const float ToogleTime = 0.0f);
	/**
	 * Controls the LED and microphone visual effects on a DualSense controller.
	 *
	 * @param ControllerId The ID of the DualSense controller to be affected.
	 * @param Value The desired LED and microphone effect to apply, represented as an ELedMicEnum value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad (Legacy)", meta = (AdvancedDisplay = "LedMicEffects Deprecated v1.2.20"))
	static void LedMicEffects(int32 ControllerId, ELedMicEnum Value);
	/**
	 * Initiates the motion sensor calibration process for the specified controller.
	 * The calibration adjusts the motion sensor sensitivity and dead zone settings.
	 *
	 * @param ControllerId The ID of the controller to be calibrated.
	 * @param Duration The duration of the calibration process in seconds.
	 * @param DeadZone The sensitivity threshold below which motion input will be ignored.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad (Legacy)", meta = (AdvancedDisplay = "MotionSensorCalibration Deprecated v1.2.20"))
	static void StartMotionSensorCalibration(
	    int32 ControllerId,
	    UPARAM(DisplayName = "Calibration Duration (Seconds)", meta = (ClampMin = "1.0", ClampMax = "10.0", UIMin = "1.0", UIMax = "10.0",
	                                                                   ToolTip = "The time in seconds to collect sensor data for calculating the stable center (baseline). Longer durations can provide a more accurate baseline.")) float Duration = 2.0f,
	    UPARAM(DisplayName = "Noise Deadzone Percentage", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0",
	                                                              ToolTip = "A percentage (0.0 to 1.0) of the sensor noise range to ignore after calibration. A higher value creates a larger deadzone, filtering out more residual noise but potentially ignoring very subtle movements.")) float DeadZone = 0.5f);
	/**
	 * Resets the gyroscope orientation for the specified controller.
	 * This aligns the gyroscope orientation to match the current physical orientation of the controller.
	 *
	 * @param ControllerId The ID of the controller whose gyroscope orientation is to be reset.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad (Legacy)", meta = (AdvancedDisplay = "GyroOrientation Deprecated v1.2.20"))
	static void ResetGyroOrientation(int32 ControllerId);
	/**
	 * Retrieves the calibration status of the motion sensor for the specified controller.
	 *
	 * @param ControllerId The ID of the controller whose motion sensor calibration status is being queried.
	 * @param Progress A reference to a variable where the calibration progress will be stored, expressed as a percentage.
	 * @return True if the calibration process is in progress, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad (Legacy)", meta = (AdvancedDisplay = "CalibrationStatus Deprecated v1.2.20"))
	static bool GetMotionSensorCalibrationStatus(int32 ControllerId, float& Progress);
	/**
	 * Enables or disables the gyroscope functionality for a specified controller.
	 *
	 * @param ControllerId The ID of the controller for which the gyroscope functionality is to be modified.
	 * @param bEnableGyroscope Set to true to enable the gyroscope, or false to disable it.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepad (Legacy)", meta = (AdvancedDisplay = "GyroscopeValues Deprecated v1.2.20"))
	static void EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope);
};
