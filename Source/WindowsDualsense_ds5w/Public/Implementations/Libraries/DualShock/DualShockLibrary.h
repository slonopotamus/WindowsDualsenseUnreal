// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "Async/TaskGraphInterfaces.h"
#include "Core/Interfaces/ISonyGamepad.h"
#include "Core/Types/Structs/Context/DeviceContext.h"
#include "CoreMinimal.h"
#include "Implementations/Libraries/Base/SonyGamepadAbstract.h"

class WINDOWSDUALSENSE_DS5W_API FDualShockLibrary : public SonyGamepadAbstract
{

public:
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
	
	virtual void UpdateInput(float Delta) override;

	/**
	 * @brief Configures the lightbar settings of the DualShock controller.
	 *
	 * This method allows control over the lightbar's color and flashing behavior on the
	 * connected DualShock controller. The color of the lightbar can be set using the
	 * specified RGB values, while the brightness time and toggle time determine its
	 * flashing pattern.
	 *
	 * @param Color An FColor object representing the desired RGB color of the lightbar.
	 * @param BrithnessTime A float value representing the duration (in seconds) for which
	 * the lightbar stays bright during a flash cycle.
	 * @param ToggleTime A float value representing the duration (in seconds) for which
	 * the lightbar is toggled off during a flash cycle.
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
