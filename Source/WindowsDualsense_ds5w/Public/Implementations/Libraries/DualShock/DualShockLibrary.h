// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "Async/TaskGraphInterfaces.h"
#include "Core/Interfaces/ISonyGamepad.h"
#include "Core/Types/Structs/Context/DeviceContext.h"
#include "Core/Types/Structs/DualShockFeatureReport.h"
#include "CoreMinimal.h"
#include "Implementations/Libraries/Base/SonyGamepadAbstract.h"

class WINDOWSDUALSENSE_DS5W_API FDualShockLibrary : public SonyGamepadAbstract
{

public:
	/**
	 * @brief Configures the settings for the DualShock controller feature report.
	 *
	 * This method accepts a feature report structure and applies the specified settings
	 * to the connected DualShock controller. It allows fine-grained control over the
	 * controller's configurations, such as input reporting and output features.
	 *
	 * @param Settings A constant reference to an FDualShockFeatureReport structure
	 * that contains the desired configuration for the DualShock controller.
	 */
	virtual void Settings(const FDualShockFeatureReport& Settings) {}
	/**
	 * @brief Initializes the DualSense library with the specified device context.
	 *
	 * This method sets up the library with the provided device context, configuring
	 * the necessary states for the DualSense controller, such as enabling or disabling
	 * certain features. It ensures the library is prepared to handle input from a connected
	 * DualSense device.
	 *
	 * @param Context A reference to an FDeviceContext structure that contains information
	 * about the current device, such as the connection type and configuration.
	 *
	 * @return Returns true if the library was successfully initialized.
	 */
	virtual bool Initialize(const FDeviceContext& Context) override;
	/**
	 * @brief Sends output data to the connected DualSense controller.
	 *
	 * This method is responsible for transmitting data to the DualSense
	 * controller through the HID device context. It ensures that the
	 * output buffering mechanism processes the data correctly before
	 * managing communication with the controller.
	 *
	 * @details The method first verifies the connection status of the HID
	 * device context. If the device is not connected, the operation is aborted.
	 * Otherwise, it delegates the responsibility of handling the output
	 * buffering to the appropriate manager, ensuring proper data flow to the device.
	 */
	virtual void UpdateOutput() override;
	/**
	 * @brief Updates the input state for a DualSense device.
	 *
	 * This method processes input data received from a DualSense controller and updates
	 * the application's state via the provided message handler. It ensures that the input
	 * is correctly mapped and associated with the specified platform user and input device.
	 *
	 * @param InMessageHandler A shared reference to the application's message handler that processes input events.
	 * @param UserId The identifier for the platform user associated with the input device.
	 * @param InputDeviceId The unique identifier of the input device to be updated.
	 * @return A boolean value indicating whether the input update was successful.
	 */
	virtual void UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	                         const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, float Delta) override;
	/**
	 * Sets the color of the lightbar on the Sony gamepad.
	 *
	 * @param Color The desired color for the lightbar, represented as an FColor object.
	 */
	virtual void SetLightbar(FColor Color, float BrithnessTime = 0.0f, float ToggleTime = 0.0f) override;
	/**
	 * Stops all currently active operations or actions associated with the interface.
	 * This method must be implemented by any derived class to handle the termination
	 * of all ongoing processes.
	 */
	virtual void ResetLights() override;
	/**
	 * Sets the vibration effect for the Sony gamepad.
	 *
	 * @param Values A reference to an FForceFeedbackValues struct containing the force feedback
	 *               intensity and duration for the vibration effect.
	 */
	virtual void SetVibration(const FForceFeedbackValues& Values) override;
};
