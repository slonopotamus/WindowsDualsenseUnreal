// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "Core/Structs/DeviceContext.h"
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Misc/CoreDelegates.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Segregations/IGamepadAudioHaptics.h"
#include "Segregations/IGamepadStatus.h"
#include "Segregations/IGamepadTouch.h"
#include "Segregations/IGamepadLightbar.h"
#include "Segregations/IGamepadSensors.h"
#include "Segregations/IGamepadTrigger.h"

/**
 * Interface defining the operations and configurations for a Sony gamepad device.
 * This interface must be implemented to interact with the gamepad, handling tasks
 * such as connection management, input updates, and device settings.
 */
class WINDOWSDUALSENSE_DS5W_API ISonyGamepad : public IGamepadStatus, public IGamepadLightbar, public IGamepadSensors, public IGamepadTouch
{

public:
	virtual ~ISonyGamepad() override = default;
	/**
	 * Retrieves the gamepad trigger interface for managing trigger-specific features.
	 *
	 * This pure virtual function provides access to the IGamepadTrigger interface.
	 * It must be implemented by classes inheriting from the ISonyGamepad interface
	 * to enable handling and configuring the trigger functionality of the gamepad.
	 *
	 * @return A pointer to the IGamepadTrigger interface instance for the gamepad.
	 */
	virtual IGamepadTrigger* GetIGamepadTrigger() = 0;
	/**
	 * Provides access to the gamepad audio haptics interface for managing audio-driven
	 * haptic feedback effects.
	 *
	 * This pure virtual function allows for the retrieval of an instance of the
	 * IGamepadAudioHaptics interface, which can be used to configure audio-based
	 * haptic feedback on the gamepad.
	 *
	 * @return A pointer to the IGamepadAudioHaptics interface instance for the gamepad.
	 */
	virtual IGamepadAudioHaptics* GetIGamepadHaptics() = 0;
	/**
	 * Initializes the gamepad library using the specified device context.
	 *
	 * This function is used to establish the initial connection and setup
	 * of the gamepad library for communication with the appropriate hardware
	 * or system interface. The `FDeviceContext` provides necessary information
	 * about the device such as its handle, connection type, and related settings.
	 *
	 * @param Context A reference to an `FDeviceContext` structure containing
	 *                the device's configuration and connection details.
	 * @return A boolean value indicating whether the library was successfully initialized.
	 *         - `true` if the initialization was successful.
	 *         - `false` if the initialization failed.
	 */
	virtual bool InitializeLibrary(const FDeviceContext& Context) = 0;
	/**
	 * Shuts down and cleans up resources related to the gamepad library.
	 *
	 * This method is responsible for properly deallocating and releasing any
	 * resources initialized or managed by the gamepad library. It should be
	 * called when the library is no longer needed to ensure no resource leaks
	 * or dangling references remain.
	 *
	 * Implementers must provide the logic to safely terminate any running
	 * processes, close device connections, and reset internal states as necessary.
	 *
	 * This function must be implemented by classes that inherit from the
	 * ISonyGamepad interface.
	 */
	virtual void ShutdownLibrary() = 0;
	/**
	 * Sets the vibration effect for the Sony gamepad.
	 *
	 * @param Values A reference to an FForceFeedbackValues struct containing the force feedback
	 *               intensity and duration for the vibration effect.
	 */
	virtual void SetVibration(const FForceFeedbackValues& Values) = 0;
	/**
	 * Provides access to the underlying mutable device context for low-level operations.
	 * Note: For advanced/console usage only. May return nullptr if not available.
	 */
	virtual FDeviceContext* GetMutableDeviceContext() = 0;
	/**
	 * Pure virtual function that sends data or commands to the connected gamepad.
	 * This function must be implemented by any class inheriting this interface.
	 */
	virtual void SendOut() = 0;
	/**
	 * Updates the input state for the Sony gamepad interface.
	 *
	 * @param InMessageHandler A shared reference to the message handler responsible for processing input events.
	 * @param UserId The platform-specific user identifier associated with the input.
	 * @param InputDeviceId The identifier for the input device being updated.
	 */
	virtual void UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, float Delta) = 0;
};
