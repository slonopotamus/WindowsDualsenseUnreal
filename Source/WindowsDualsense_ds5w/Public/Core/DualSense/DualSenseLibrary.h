// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "Async/TaskGraphInterfaces.h"
#include "Containers/Queue.h"
#include "Core/Interfaces/ISonyGamepad.h"
#include "Core/Interfaces/Segregations/IGamepadAudioHaptics.h"
#include "Core/Interfaces/Segregations/IGamepadTrigger.h"
#include "Core/Enums/EDeviceCommons.h"
#include "Core/Structs/DeviceContext.h"
#include "Core/Structs/DualSenseFeatureReport.h"
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include <atomic>



/**
 * @class FTouchPoint1
 * @brief Represents a touch point in a touch-based input system.
 *
 * The FTouchPoint1 class is designed to encapsulate the properties and behavior
 * of a single touch point detected on a touch-sensitive surface. It provides
 * information such as the position, pressure, and state of the touch point,
 * which can be used to build touch interactions within an application.
 *
 * This class is typically used in systems that handle multi-touch input,
 * providing the data necessary to track individual touch points and their
 * specific attributes.
 *
 * @details The class may store information about the position of the touch point
 * in screen coordinates, the pressure or force of the touch, and the current state
 * of the touch event (e.g., touch began, moved, or ended). It may be used in
 * conjunction with other similar touch point objects for multi-touch gesture recognition
 * or input processing.
 */
struct FTouchPoint1
{
	/**
	 * @brief Represents a generic variable `X`.
	 *
	 * This variable is used as a placeholder or for storing a specific value
	 * during the program's execution. The exact nature and purpose of `X`
	 * depend on the context in which it is defined.
	 *
	 * @note Ensure that the value assigned to `X` is valid and adheres
	 * to the expected data type or constraints for its proper usage.
	 */
	uint16_t X;
	/**
	 * @brief Computes the factorial of a given non-negative integer.
	 *
	 * This function calculates the factorial of a number using recursion.
	 * The factorial of a number `n` is the product of all positive integers less than
	 * or equal to `n`. Factorial is defined as:
	 * - 0! = 1
	 * - n! = n * (n-1)!, where n > 0
	 *
	 * @param n The non-negative integer for which the factorial is to be computed.
	 * @return The factorial of the input number. If the input is 0, returns 1.
	 * @throw std::invalid_argument If the input is a negative number.
	 */
	uint16_t Y;
	/**
	 * @brief Represents a downward movement in a grid or coordinate system.
	 *
	 * This class encapsulates the functionality related to movement in the downward direction.
	 * It could be used in contexts such as 2D games, simulations, or navigation systems
	 * where positioning within a coordinate grid needs to be adjusted downward.
	 */
	bool Down;
	/**
	 * @class Id
	 * @brief Represents a unique identifier.
	 *
	 * This class is designed to handle and manipulate unique identifiers
	 * for various use cases. It provides functionality to generate,
	 * validate, and compare unique ID values.
	 *
	 * @details
	 * The Id class can be used in scenarios where unique identification
	 * is required, such as database keys, UUID generation, and other
	 * similar functionalities. It ensures that each instance represents
	 * a distinct identifier.
	 */
	unsigned char Id;
};

/**
 * @class FTouchPoint2
 * @brief Represents a point of contact on a touch-sensitive surface with additional properties.
 *
 * The FTouchPoint2 class is used to encapsulate the state and properties of a touch point
 * as it interacts with a touch-sensitive interface. Each instance of this class provides
 * detailed information about the current state of the touch point, including its position,
 * pressure, and movement. This is typically used in multi-touch systems to track and manage
 * touch inputs across a surface.
 *
 * The FTouchPoint2 class is particularly useful in applications like gesture recognition,
 * gaming, and graphical applications that require an understanding of user interactions
 * on touch input devices.
 */
struct FTouchPoint2
{
	/**
	 * @brief Represents the variable X, which is used as a placeholder or for a specific purpose in the context of the program.
	 *
	 * This variable may hold data or serve as a control mechanism
	 * depending on the logic implemented in the application. The detailed
	 * behavior, intent, and value assignment of X should align with the program's requirements.
	 *
	 * @note Ensure proper initialization and context-specific usage of X to avoid
	 * unintended behaviors.
	 */
	uint16_t X;
	/**
	 * @brief Represents a variable 'Y' with unspecified type and purpose.
	 *
	 * This variable is declared as 'Y' and may be assigned or used in various contexts
	 * depending on the implementation or application where it is defined. The specific
	 * type, purpose, and usage of 'Y' is undefined and should be interpreted based on
	 * its associated logic or framework.
	 */
	uint16_t Y;
	/**
	 * @brief Represents a direction or movement towards a lower position or level.
	 *
	 * The variable Down is typically used to indicate a downward movement,
	 * orientation, or state in various contexts such as navigation, positioning,
	 * or directional control in a program.
	 */
	bool Down;
	/**
	 * @brief Represents a unique identifier.
	 *
	 * This variable is used to store a unique value that can be assigned
	 * to distinguish between different entities or objects in a system.
	 * It is typically used for identification purposes.
	 */
	unsigned char Id;
};

/**
 * @class FAccelerometer
 * @brief Represents an accelerometer component that tracks and provides data related to acceleration.
 *
 * The FAccelerometer class is responsible for handling acceleration readings
 * and providing utility functions to process and retrieve those readings in a
 * meaningful way. This class can be used to monitor motion in three-dimensional
 * space and calculate relevant metrics based on the gathered data.
 *
 * @details
 * The accelerometer typically measures acceleration in three axes: X, Y, and Z,
 * and provides functionality to retrieve raw acceleration values, calculate the
 * magnitude of the acceleration vector, and filter out noise or apply smoothing
 * when needed. This class can be utilized for physical simulation, motion
 * detection, or other use cases where acceleration tracking is required.
 */
struct FAccelerometer
{
	/**
	 *
	 */
	int16_t X;
	/**
	 * @brief Represents the variable Y used within the application.
	 *
	 * This variable serves as a placeholder for a specific purpose
	 * depending on the context of the application. The exact use of
	 * Y should be defined in the code implementation and may represent
	 * a variety of data types or values depending on its usage.
	 *
	 * The role of Y can vary, such as acting as an input parameter,
	 * intermediate storage, or output result. Proper initialization
	 * and handling of this variable are essential to ensure the correctness
	 * and reliability of the program's operations.
	 *
	 * Ensure that the value of Y is properly documented and updated
	 * throughout the codebase to avoid confusion or misuse.
	 */
	int16_t Y;
	/**
	 * @brief Represents a variable named Z.
	 *
	 * This variable is designed to hold a specific value or data type as required
	 * by the implementation. Its intended use and functionality should be defined
	 * within the corresponding program or module context. The type and purpose of
	 * this variable should align with the logic and requirements of its usage.
	 *
	 * Ensure that the value assigned to Z adheres to the appropriate constraints
	 * or expectations in the application to maintain correctness and stability.
	 */
	int16_t Z;
};

/**
 * @class FGyro
 * @brief The FGyro class represents a gyroscopic sensor that provides data on angular velocity and orientation in three-dimensional space.
 *
 * This class models the behavior and properties of a gyroscope, commonly used in robotics, mobile devices, and game controllers for motion tracking
 * and orientation detection. The class offers functionalities to retrieve, reset, and process gyroscopic readings.
 *
 * Detailed methods and attributes allow users to interact with the gyroscope hardware or simulation to acquire real-time data for use in various applications.
 * The gyroscope data includes angular velocity along the X, Y, and Z axes, as well as possible methods for calibration.
 */
struct FGyro
{
	/**
	 * @brief Represents a variable used for general purposes within the application.
	 *
	 * This variable may store values of various types based on its application context,
	 * and its purpose is defined by the implementation details of the code where it is used.
	 *
	 * @note Ensure to initialize and manage the value of this variable correctly to avoid
	 * unexpected behavior or runtime errors.
	 */
	int16_t X;
	/**
	 * @brief Represents a generic variable or entity identified as 'Y'.
	 *
	 * @details The purpose and type of 'Y' should be defined by its context in
	 *          the codebase. It could serve as a placeholder or functional part
	 *          of a process and requires further clarification or initialization
	 *          during implementation.
	 *
	 * @note Ensure 'Y' is properly initialized and used consistently to avoid
	 *       unexpected behavior. Misuse of 'Y' may lead to runtime errors or
	 *       logical inconsistencies.
	 */
	int16_t Y;
	/**
	 * @brief Represents a variable or entity denoted as Z.
	 *
	 * This variable may serve as a placeholder or represent a specific value,
	 * object, or function in the given context. Ensure to initialize or define Z
	 * appropriately before using it, based on its intended purpose.
	 *
	 * @note The specific use case and type of Z must be determined by its context
	 *       within the scope of the application or module.
	 */
	int16_t Z;
};

struct FSensorBounds
{
	FVector2D Gyro_X_Bounds; // X = Min, Y = Max

	FVector2D Gyro_Y_Bounds; // X = Min, Y = Max

	FVector2D Gyro_Z_Bounds; // X = Min, Y = Max

	FVector2D Accel_X_Bounds; // X = Min, Y = Max

	FVector2D Accel_Y_Bounds; // X = Min, Y = Max

	FVector2D Accel_Z_Bounds; // X = Min, Y = Max

	FSensorBounds()
	{
		Gyro_X_Bounds = FVector2D(FLT_MAX, -FLT_MAX);
		Gyro_Y_Bounds = FVector2D(FLT_MAX, -FLT_MAX);
		Gyro_Z_Bounds = FVector2D(FLT_MAX, -FLT_MAX);
		Accel_X_Bounds = FVector2D(FLT_MAX, -FLT_MAX);
		Accel_Y_Bounds = FVector2D(FLT_MAX, -FLT_MAX);
		Accel_Z_Bounds = FVector2D(FLT_MAX, -FLT_MAX);
	}
};

/**
 * @class FDualSenseLibrary
 * @brief Utility class for interfacing with the PlayStation DualSense controller.
 *
 * This class provides a collection of static functions and utilities specific to
 * interacting with the PlayStation DualSense controller within a project. It facilitates
 * operations such as managing haptic feedback, adaptive triggers, and other special
 * features offered by the controller.
 *
 * The UDualSenseLibrary is typically used to control and respond to events from
 * a connected DualSense device, enabling advanced functionalities for an enhanced
 * user experience.
 *
 * Features supported may include:
 * - Triggering haptic feedback.
 * - Configuring adaptive trigger effects.
 * - Managing light bars and other controller-specific outputs.
 * - Retrieving connected-state and other metadata.
 *
 * The library is designed for developers seeking to leverage unique features of
 * the DualSense controller programmatically within an application.
 */
class WINDOWSDUALSENSE_DS5W_API FDualSenseLibrary : public ISonyGamepad, public IGamepadTrigger, public IGamepadAudioHaptics
{

public:
	FDualSenseLibrary() : bEnableTouch(false), bWasTouch1Down(false), bWasTouch2Down(false), HasPhoneConnected(false), LevelBattery(0), LeftTriggerFeedback(0), RightTriggerFeedback(0), bEnableAccelerometerAndGyroscope(false), bHasMotionSensorBaseline(false), bIsCalibrating(false), CalibrationStartTime(0), CalibrationDuration(0), CalibrationSampleCount(0), AudioVibrationSequence(0)
	{
	}

	virtual IGamepadTrigger* GetIGamepadTrigger() override
	{
		return this;
	}

	virtual IGamepadAudioHaptics* GetIGamepadHaptics() override
	{
		return this;
	}

	/**
	 * @brief Configures device settings for a connected device.
	 *
	 * This method allows customization and application of configuration
	 * settings for a specific device by modifying its settings object.
	 * It is typically overridden to provide device-specific configuration
	 * logic.
	 *
	 * @param Settings A reference to a FDeviceSettings object that holds
	 * the device's configuration options to be adjusted or updated.
	 */
	virtual void Settings(const FDualSenseFeatureReport& Settings);
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
	virtual bool InitializeLibrary(const FDeviceContext& Context) override;
	/**
	 * @brief Shuts down the DualSense library and releases related resources.
	 *
	 * This method is responsible for halting the operation of the DualSense library,
	 * including resetting button states, clearing device contexts, and releasing
	 * system handles. It ensures that all resources utilized by the library are
	 * properly deallocated to prevent potential memory leaks or resource conflicts.
	 *
	 * @details The method resets the internal button states, zeroes any output data
	 * associated with the HID device context, closes the associated device handle,
	 * and frees any allocated HID device contexts via the DualSense HID Manager.
	 * Once completed, a log message is generated to confirm the successful shutdown.
	 */
	virtual void ShutdownLibrary() override;
	/**
	 * @brief Checks if a DualSense device is currently connected.
	 *
	 * This method determines whether a DualSense controller is actively connected
	 * to the system. It queries the underlying device context to assess the connection
	 * status and provides a boolean value indicating the result.
	 *
	 * @return True if a DualSense controller is connected; otherwise, false.
	 */
	virtual bool IsConnected() override;
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
	virtual void SendOut() override;
	/**
	 * @brief Handles button input events for a DualSense controller.
	 *
	 * This function checks the current state of a button on a DualSense controller and triggers
	 * appropriate input events (button press or release) based on the change in its state.
	 *
	 * @param InMessageHandler The message handler responsible for dispatching input events.
	 * @param UserId The platform user ID associated with the controller.
	 * @param InputDeviceId The unique identifier for the DualSense input device.
	 * @param ButtonName The name of the button being checked.
	 * @param IsButtonPressed A boolean indicating the current pressed state of the button (true if pressed, false otherwise).
	 */
	virtual void CheckButtonInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
	                              const FPlatformUserId UserId, const FInputDeviceId InputDeviceId,
	                              const FName ButtonName, const bool IsButtonPressed);
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
	 * Stops any ongoing adaptive trigger effects on the specified controller hand.
	 *
	 * @param Hand The hand for which to stop the adaptive trigger effect.
	 *             Acceptable values are EControllerHand::Left, EControllerHand::Right,
	 *             or EControllerHand::AnyHand.
	 */
	virtual void StopTrigger(const EControllerHand& Hand) override;
	/**
	 * @brief Configures the trigger settings on a DualSense controller for GameCube-style behavior.
	 *
	 * This method configures the triggers on the DualSense controller to emulate
	 * a GameCube-style response based on the specified controller hand.
	 * It adjusts the trigger's mode and strengths for the desired effects.
	 *
	 * @param Hand The hand (left, right, or both) corresponding to the controller
	 * side where the GameCube-style trigger behavior should be applied. Must be
	 * a value of the EControllerHand enumeration.
	 */
	virtual void SetGameCube(const EControllerHand& Hand) override;
	/**
	 * @brief Sets the trigger resistance properties for a specific controller hand.
	 *
	 * This method configures the adaptive trigger resistance on a controller,
	 * specifying the starting zones and strength of the resistance based on input parameters.
	 * It applies the settings to the designated controller hand, allowing for precise
	 * customization of trigger feedback in gameplay or applications.
	 *
	 * @param StartZones Specifies the starting zones in which the resistance should be applied.
	 * @param Strength Determines the intensity or strength of the resistance.
	 * @param Hand Indicates the controller hand (e.g., left or right) where the resistance should be configured.
	 */
	virtual void SetResistance(uint8 StartZones, uint8 Strength, const EControllerHand& Hand) override;
	/**
	 * @brief Configures the bow effect settings on a DualSense controller.
	 *
	 * This method allows customization of the bow effect by specifying its start zone,
	 * behavior, force amplitude, and the hand associated with the action.
	 *
	 * @param StartZone The starting zone value for the bow effect.
	 * @param SnapBack The SnapBack of the force applied during the bow effect.
	 * @param Hand The controller hand (left or right) associated with the bow action.
	 */
	virtual void SetBow22(uint8 StartZone, uint8 SnapBack, const EControllerHand& Hand) override;
	/**
	 * @brief Configures the galloping trigger feedback behavior on a DualSense controller.
	 *
	 * This method sets up the trigger effects on the specified hand of a DualSense controller
	 * to emulate a "galloping" feedback pattern. It allows customization of start and end positions,
	 * the characteristics of two feedback "foot" pressures, and the frequency of the effect.
	 *
	 * @param StartPosition The beginning position of the galloping effect, represented as a 0-based index.
	 * @param EndPosition The ending position of the galloping effect, represented as a 0-based index.
	 * @param FirstFoot The feedback strength value for the first foot during the galloping effect, ranging from 0-8.
	 * @param SecondFoot The feedback strength value for the second foot during the galloping effect, ranging from 0-8.
	 * @param Frequency The frequency of the galloping effect, determining how rapidly it alternates or triggers.
	 * @param Hand Specifies the controller hand (left, right, or any) to apply the galloping effect.
	 */
	virtual void SetGalloping23(uint8 StartPosition, uint8 EndPosition, uint8 FirstFoot, uint8 SecondFoot, uint8 Frequency, const EControllerHand& Hand) override;
	/**
	 * @brief Configures the trigger effect for the DualSense controller's adaptive triggers.
	 *
	 * This method sets the advanced bow effect (Mode 0x25) for the specified controller triggers,
	 * allowing customization of trigger resistance and behavior for enhanced gaming experiences.
	 *
	 * @param StartZone Specifies the starting position of the trigger effect.
	 * @param Amplitude Defines the amplitude or intensity of the resistance effect.
	 * @param Behavior Determines the behavior of the effect in terms of responsiveness and resistance.
	 * @param Trigger Specifies an additional parameter for customizing the effect behavior.
	 * @param Hand Determines which hand (Left, Right, or AnyHand) the configuration applies to.
	 */
	virtual void SetWeapon25(uint8 StartZone, uint8 Amplitude, uint8 Behavior, uint8 Trigger, const EControllerHand& Hand) override;
	/**
	 * @brief Configures the machine gun effect on a DualSense controller.
	 *
	 * This method controls the haptic feedback pattern to simulate a machine gun-like vibration effect on the controller.
	 * It allows setting parameters such as the starting zone, behavior, amplitude, frequency, and the specific controller hand.
	 *
	 * @details The method enables developers to provide a highly immersive experience by fine-tuning the haptic responses.
	 * By adjusting the starting zone and behavior, the effect can be customized to simulate different mechanical sensations,
	 * while the amplitude and frequency control the intensity and oscillation of the feedback.
	 *
	 * @param StartZone Specifies the initial zone for the haptic effect.
	 * @param Behavior Defines the behavioral pattern of the machine gun effect.
	 * @param Amplitude Sets the vibration amplitude, determining the strength of the feedback.
	 * @param Frequency Configures the frequency of the haptic pulses to simulate firing intervals.
	 * @param Hand Indicates which controller hand (left or right) will receive the effect.
	 */
	virtual void SetMachineGun26(uint8 StartZone, uint8 Behavior, uint8 Amplitude, uint8 Frequency, const EControllerHand& Hand) override;
	/**
	 * @brief Configures the advanced machine effect (Mode 0x27) for DualSense controller triggers.
	 *
	 * This function sets the parameters for the Mode 0x27 advanced effect on the adaptive triggers
	 * of a DualSense controller. It specifies the behavior of the effect, including starting zone,
	 * force, amplitude, period, and frequency, and applies these configurations to the left, right,
	 * or both triggers based on the specified hand.
	 *
	 * @param StartZone Specifies the starting zone of the trigger effect. Defines the point where the effect begins.
	 * @param BehaviorFlag Indicates the behavior type of the trigger effect. Determines specific effect adjustments (e.g., feedback type).
	 * @param Force Defines the level of force to apply in the trigger effect. Higher values result in stronger feedback.
	 * @param Amplitude Sets the amplitude of the trigger vibration or feedback pattern.
	 * @param Period Specifies the period for the trigger effect, controlling timing intervals or pulse duration.
	 * @param Frequency Determines the frequency for the trigger vibration or repeated effect.
	 * @param Hand Identifies the controller hand (left, right, or both) for applying the trigger effect.
	 */
	virtual void SetMachine27(uint8 StartZone, uint8 BehaviorFlag, uint8 Force, uint8 Amplitude, uint8 Period, uint8 Frequency, const EControllerHand& Hand) override;
	/**
	 * @brief Sets custom trigger behavior for the specified controller hand using custom hexadecimal byte data.
	 *
	 * This method allows developers to define custom behaviors for the trigger buttons on a DualSense controller.
	 * The behavior is specified using an array of 10 hexadecimal byte strings which represent configuration and
	 * behavior codes that are sent directly to the controller.
	 *
	 * @param Hand The hand designation of the controller (e.g., left, right, or any hand) for which the custom trigger behavior is applied.
	 * @param HexBytes An array of hexadecimal byte strings defining the custom trigger configuration. Must contain exactly 10 valid values.
	 */
	virtual void SetCustomTrigger(const EControllerHand& Hand, const TArray<FString>& HexBytes) override;
	/**
	 * Retrieves the current battery level of the DualSense controller.
	 *
	 * @return The battery level as a floating-point value, representing the charge percentage or level.
	 */
	virtual float GetBattery() const override
	{
		return LevelBattery;
	}

	/**
	 * Sets the LED player indicator effects based on the desired player LED pattern and brightness intensity.
	 *
	 * @param Led The LED pattern to be displayed on the device, represented by the ELedPlayerEnum enumeration.
	 * @param Brightness The brightness intensity for the LED, represented by the ELedBrightnessEnum enumeration.
	 */
	virtual void SetPlayerLed(ELedPlayerEnum Led, ELedBrightnessEnum Brightness) override;
	/**
	 * Sets the microphone LED effects on the DualSense controller.
	 *
	 * @param Led The desired LED microphone effect to be applied. It is defined by the ELedMicEnum enumeration,
	 *               which includes options such as MicOff, MicOn, and Pulse.
	 */
	virtual void SetMicrophoneLed(ELedMicEnum Led) override;
	/**
	 * @brief Sets the lightbar color and behavior on the device.
	 *
	 * This method allows customizing the lightbar by specifying its color,
	 * brightness transition duration, and toggle interval. It is used
	 * to control the visual feedback on devices that have a lightbar feature.
	 *
	 * @param Color The desired color of the lightbar, represented as an FColor.
	 * @param BrithnessTime The duration for transitioning the brightness, in seconds.
	 * @param ToggleTime The duration for toggling the lightbar on and off, in seconds.
	 */
	virtual void SetLightbar(FColor Color, float BrithnessTime = 0.0f, float ToggleTime = 0.0f) override;
	/**
	 * @brief Updates the vibration feedback for a DualSense controller using force feedback values.
	 *
	 * This method takes in force feedback values and applies the corresponding vibration settings
	 * to the DualSense controller's left and right motors. The vibration levels for each motor are
	 * computed based on the provided force feedback values and are sent to the device to update
	 * its output state.
	 *
	 * @param Vibration The force feedback values representing the intensity of vibrations for the
	 * left and right motors. These values determine how strong the vibrations will be for the
	 * respective motors.
	 */
	virtual void SetVibration(const FForceFeedbackValues& Vibration) override;
	/**
	 * @brief Stops all ongoing input and feedback operations on the DualSense controller.
	 *
	 * The StopAll function halts all active feedback features such as LED lights,
	 * trigger feedback, and other customizable components on the DualSense controller.
	 * It resets the state of the controller's visual and haptic features.
	 *
	 * @details This function modifies the controller's state by turning off the brightness
	 * of the player LED, deactivating lightbar and trigger modes, and updating the lightbar
	 * colors based on the assigned ControllerID. Each ControllerID is associated with a
	 * specific lightbar color to represent different controller states.
	 *
	 * The function ensures consistent controller deactivation for any connected players
	 * by resetting features to their default state and immediately sending the state update
	 * to the hardware. This can be used in scenarios where the controller must be brought
	 * into a neutral state, such as when pausing gameplay or shutting down the system.
	 */
	virtual void ResetLights() override;
	/**
	 * @brief Retrieves the connection type of the device.
	 *
	 * This method provides information about the connection type of the device,
	 * such as whether it is connected via USB, Bluetooth, or another supported
	 * connection type. It overrides the base class implementation to return
	 * the specific connection type associated with the HID device context.
	 *
	 * @return The connection type of the device as an enumerated value of type EDeviceConnection.
	 */
	virtual EDeviceConnection GetConnectionType() override
	{
		return HIDDeviceContexts.ConnectionType;
	}

	/**
	 * @brief Retrieves the type of device associated with the current context.
	 *
	 * This method provides the specific type of device being used, as represented
	 * by the associated device type enumeration. The retrieved value can be utilized
	 * to determine the characteristics or capabilities of the device in use.
	 *
	 * @return The device type as an enumeration value of EDeviceType.
	 */
	virtual EDeviceType GetDeviceType() override
	{
		return HIDDeviceContexts.DeviceType;
	}

	/**
	 * @brief Enables or disables the touch functionality for the DualSense library.
	 *
	 * This method is used to control whether the touch functionality within
	 * the DualSense controller is active or inactive. When enabled, touch
	 * inputs will be processed and can be used in the application.
	 *
	 * @param bIsTouch A boolean value that determines the touch functionality state.
	 *                 Set to true to enable touch or false to disable it.
	 */
	virtual void EnableTouch(const bool bIsTouch) override;
	/**
	 * @brief Enables or disables the motion sensor feature of the DualSense controller.
	 *
	 * This function allows the user to toggle the motion sensor functionality of
	 * the DualSense controller on or off according to their application's requirements.
	 *
	 * @param bIsMotionSensor A boolean value that specifies whether the motion sensor should be enabled (true)
	 * or disabled (false).
	 */
	virtual void EnableMotionSensor(bool bIsMotionSensor) override;
	/**
	 * @brief Initiates the calibration process for the DualSense motion sensors.
	 *
	 * This method starts the calibration routine for the motion sensors in a DualSense controller.
	 * It collects raw sensor data over the specified duration to determine baseline values and applies
	 * a dead zone to reduce noise or unintentional drift in sensor readings.
	 *
	 * The calibration process allows for improved precision in motion-related computations
	 * by compensating for sensor drift and other inaccuracies.
	 *
	 * @param Duration The duration, in seconds, to collect sensor data for calibration.
	 *                 Values are clamped between 1.0 and 10.0 seconds.
	 * @param DeadZone The threshold for sensor dead zone to filter out small motion or drift.
	 *                 Valid range is from 0.0 to 1.0.
	 */
	virtual void StartMotionSensorCalibration(float Duration, float DeadZone) override;
	/**
	 * Retrieves the current calibration status of the motion sensors.
	 *
	 * @param OutProgress A reference to a float where the current calibration progress will be stored.
	 *                    The value ranges from 0.0 (no progress) to 1.0 (fully calibrated).
	 * @return True if the calibration status was successfully retrieved, false otherwise.
	 */
	virtual bool GetMotionSensorCalibrationStatus(float& OutProgress) override;
	/**
	 * @brief Updates the haptic feedback system of the DualSense controller with audio data.
	 *
	 * The AudioHapticUpdate method sends audio-based haptic feedback data to the DualSense controller.
	 * It handles encoding and transmitting audio data to produce haptic vibration effects
	 * on the controller hardware.
	 *
	 * @param Data A byte array containing the audio haptic data to be transmitted.
	 * The method processes up to a maximum of 64 bytes of this data.
	 *
	 * @details This function interacts with the device context to check if the controller is connected,
	 * processes the provided audio data into the appropriate format, and forwards it to the
	 * DualSense hardware. The method ensures memory-safe data handling and integrates with
	 * advanced communication protocols to achieve synchronized haptic feedback.
	 *
	 * This functionality is typically implemented in systems that aim to provide immersive
	 * feedback during audio playback or gaming scenarios that utilize DualSense controllers.
	 */
	virtual void AudioHapticUpdate(TArray<int8> Data) override;
	/**
	 * @brief Resets the gyro orientation to its default alignment.
	 *
	 * This method reinitializes the gyroscope orientation, typically
	 * to recalibrate it or correct drift. It should be invoked when
	 * a new reference orientation is needed.
	 */
	virtual void ResetGyroOrientation() override;
	/**
	 * @brief Retrieves a mutable device context associated with the object.
	 *
	 * This method provides access to the device context in a mutable form, allowing modifications to the underlying
	 * hardware interface descriptor (HID) data specific to the device.
	 *
	 * @return A pointer to the mutable FDeviceContext object corresponding to the device's HID context.
	 */
	virtual FDeviceContext* GetMutableDeviceContext() override { return &HIDDeviceContexts; }
	/**
	 * A map representing the states of various buttons on a controller.
	 *
	 * Each key in the map is a button name (FName), and its associated value is a boolean
	 * indicating whether the button is currently pressed (`true`) or not pressed (`false`).
	 *
	 * This variable is primarily used for tracking button input states and ensuring
	 * accurate representation of input events, such as detecting when a button's state
	 * changes from pressed to released or vice versa.
	 *
	 * The map is updated dynamically during controller runtime, including in functions
	 * like CheckButtonInput, which ensures real-time synchronization of input states.
	 * It is reset during library shutdown to clear all stored button states.
	 */
	TMap<const FName, bool> ButtonStates;
	/**
	 * @typedef AnalogStates
	 * @brief Represents a mapping of analog input states in the DualSense library.
	 *
	 * AnalogStates is a container that maps unique input identifiers, represented by FName,
	 * to their respective float values, which typically denote the state or intensity of analog inputs.
	 *
	 * This map is used to handle and store the state of analog inputs, such as triggers or sticks,
	 * providing a mechanism to track their values for input handling or processing purposes in an application.
	 *
	 * @details The keys in this map (FName) are designed to uniquely identify different analog input sources,
	 * while the associated float values represent their corresponding state, usually on a normalized scale.
	 */
	TMap<const FName, float> AnalogStates;

protected:
	/**
	 * @brief The PlatformInputDeviceMapper is responsible for mapping platform-specific
	 *        input device data to a standardized format used within the application.
	 *
	 * This variable is part of the input handling system and plays a crucial role in
	 * ensuring compatibility between different platform input devices and the application's
	 * input handling logic. It abstracts the differences in input device representations
	 * across operating systems or platforms, providing a unified interface for accessing
	 * input data.
	 *
	 * Typical use cases include mapping device identifiers, translating input codes, and
	 * adapting platform-specific input behavior to match application requirements.
	 *
	 * The PlatformInputDeviceMapper handles tasks such as:
	 * - Mapping raw input device identifiers to application-specific identifiers.
	 * - Normalizing input event data across diverse platforms and devices.
	 * - Facilitating cross-platform input device compatibility.
	 */
	static FGenericPlatformInputDeviceMapper PlatformInputDeviceMapper;
	/**
	 * Sets the connection state of a phone.
	 *
	 * @param HasConnected Indicates whether the phone is connected (true) or not (false).
	 */
	void SetHasPhoneConnected(bool HasConnected);
	/**
	 * Sets the battery level of the DualSense device.
	 *
	 * @param Level The current battery level as a float value.
	 * @param FullyCharged Indicates if the battery is fully charged.
	 * @param Charging Indicates if the device is currently charging.
	 */
	void SetLevelBattery(float Level, bool FullyCharged, bool Charging);

private:
	/**
	 * @typedef FPlatformUserId
	 * @brief Represents a platform-specific user identifier.
	 *
	 * The FPlatformUserId type is used to uniquely identify a user on the platform.
	 * It helps in associating specific actions, settings, or preferences with a
	 * particular user in a platform environment.
	 *
	 * @details Often utilized in multi-user applications or systems, this identifier
	 * allows differentiation between users and helps in maintaining user-specific
	 * data or context, especially in scenarios involving shared devices or services.
	 */
	FPlatformUserId DSUserId;
	/**
	 * @brief A variable that indicates whether touch functionality is enabled or disabled.
	 *
	 * This variable is used to toggle the touch input capability of the system or application.
	 * When set to true, touch input is enabled, allowing the system to respond to touch events.
	 * When set to false, touch input is disabled, and touch interactions are ignored.
	 */
	bool bEnableTouch;
	/**
	 * @brief Indicates whether the first touch input was active in the previous frame.
	 *
	 * Represents a boolean state that tracks if the first touch input (Touch1)
	 * was detected as being down (pressed or active) during the last frame.
	 * This variable is useful for identifying changes in touch state, such as
	 * transitions between touch down and touch up events, or for implementing
	 * touch-based controls in applications.
	 *
	 * @details This flag is often utilized within systems handling touch input
	 * to determine the prior state of a touch point. By comparing its current
	 * and previous states, developers can detect touch events such as presses,
	 * holds, or releases for the primary touch interaction.
	 */
	bool bWasTouch1Down;
	/**
	 * @variable bWasTouch2Down
	 * @brief Indicates the previous state of the second touch input on a touch-sensitive device.
	 *
	 * bWasTouch2Down is a boolean flag used to track whether the second touch input
	 * on a touch-sensitive surface was previously in the "down" state. It stores the
	 * state from a previous update, allowing for comparison with the current state
	 * to detect changes in the touch interaction.
	 *
	 * @details This variable is typically part of an input handling system, where it plays
	 * a role in monitoring touch events, such as detecting when the second touch point
	 * transitions between active (down) and inactive (up) states. It may be utilized
	 * in gesture recognition or other forms of touch-based interaction management.
	 */
	bool bWasTouch2Down;
	/**
	 * Indicates whether a phone is connected to the system.
	 *
	 * This variable is used to store the connection status of a phone.
	 * It is updated by the SetHasPhoneConnected method.
	 * - `true`: A phone is connected.
	 * - `false`: No phone is connected.
	 */
	bool HasPhoneConnected;
	/**
	 * @brief Represents the current level of the battery.
	 *
	 * This variable is used to store the battery percentage or charge level,
	 * typically as an integer or floating-point value. It can be used to
	 * monitor battery status in various applications and scenarios.
	 *
	 * Note: The scale and representation (e.g., percentage, voltage) should
	 * be specified in the implementation or context where the variable is used.
	 */
	float LevelBattery;
	/**
	 * @brief Represents the feedback intensity for the left trigger of a DualSense controller.
	 *
	 * This variable is used to store the current level of feedback applied to the left trigger (L2 button).
	 * The value typically ranges from 0.0 to 1.0, where 0.0 signifies no feedback and 1.0 represents maximum feedback intensity.
	 */
	float LeftTriggerFeedback;
	/**
	 * @brief Represents the feedback intensity or state for the right trigger in a controller.
	 *
	 * This variable is used to specify or retrieve the vibration/feedback level
	 * applied to the right trigger, which can enhance user experience during gameplay
	 * or interactive scenarios by providing tactile feedback.
	 *
	 * The range and interpretation of the value may depend on the specific implementation
	 * or hardware being used.
	 */
	float RightTriggerFeedback;
	/**
	 * @variable SensorsDeadZone
	 * @brief Defines the threshold for ignoring small sensor input variations.
	 *
	 * SensorsDeadZone is used to eliminate unintended small variations or noise
	 * in sensor readings by setting a minimum threshold value. Any input changes
	 * below this value are considered insignificant and are ignored in further
	 * processing.
	 *
	 * @details This variable is particularly useful for fine-tuning input systems
	 * to ensure smoother and more reliable sensor-based interactions by reducing
	 * the sensitivity to unintentional micro-adjustments. It is often applied in
	 * joystick or motion sensor implementations.
	 */
	float SensorsDeadZone = 0.0f;
	/**
	 * @variable SensorsDeadZone
	 * @brief Defines the threshold for ignoring small sensor input variations.
	 *
	 * SensorsDeadZone is used to eliminate unintended small variations or noise
	 * in sensor readings by setting a minimum threshold value. Any input changes
	 * below this value are considered insignificant and are ignored in further
	 * processing.
	 *
	 * @details This variable is particularly useful for fine-tuning input systems
	 * to ensure smoother and more reliable sensor-based interactions by reducing
	 * the sensitivity to unintentional micro-adjustments. It is often applied in
	 * joystick or motion sensor implementations.
	 */
	float AnalogDeadZone = 0.3f;
	/**
	 * @variable EnableAccelerometerAndGyroscope
	 * @brief Flags the activation of accelerometer and gyroscope sensors in the system.
	 *
	 * This variable determines whether the accelerometer and gyroscope functionalities
	 * are enabled for the system. When set to true, data from these sensors will be collected
	 * and utilized, typically for motion detection or orientation tracking.
	 *
	 * @details This flag is often used in systems that require motion input for functionality,
	 * such as gaming controllers, virtual reality devices, or motion-sensing applications.
	 * Disabling this may reduce resource usage but will disable motion-based features.
	 */
	bool bEnableAccelerometerAndGyroscope;
	/**
	 * @var bIsResetGyroscope
	 * @brief Indicates whether the gyroscope reset operation is enabled.
	 *
	 * This boolean variable is used to determine if the gyroscope should be reset
	 * to its default state. When set to true, the system will perform the necessary
	 * operations to reset and recalibrate the gyroscope. When set to false, the
	 * gyroscope continues operating without reset.
	 *
	 * @details The variable is typically utilized in applications or systems
	 * where gyroscope functionality is integrated, such as motion sensing
	 * for gaming controllers, virtual reality devices, or other motion-driven
	 * systems. Resetting the gyroscope might be necessary in scenarios where
	 * recalibration is required due to drift or unexpected behavior.
	 */
	bool bIsResetGyroscope = false;
	/**
	 * @brief Indicates the presence of a motion sensor baseline calibration.
	 *
	 * The bHasMotionSensorBaseline variable is used to determine whether
	 * a baseline calibration has been established for the motion sensor.
	 * This is important for ensuring reliable readings and performance
	 * from the motion sensor in applications that depend on accurate
	 * motion or orientation data.
	 *
	 * @details A value of true indicates that a baseline is present, suggesting
	 * that the motion sensor is calibrated and ready for precise operation.
	 * A value of false indicates that no baseline calibration exists,
	 * signaling that calibration might be required or motion sensor
	 * readings could be unreliable.
	 */
	bool bHasMotionSensorBaseline;
	/**
	 * @brief Indicates whether the system is currently in the process of calibration.
	 *
	 * The bIsCalibrating flag is used to track if a calibration operation is active.
	 * Calibration procedures are often necessary to ensure accurate performance of
	 * input devices or sensors, and this property serves as a state indicator during
	 * such processes.
	 *
	 * @details While true, the system may be engaged in activities that adjust
	 * or fine-tune hardware or software settings based on specific calibration data.
	 * This information can be used to manage or modify application behavior during
	 * these operations, ensuring no conflicts arise while calibration is underway.
	 */
	bool bIsCalibrating;
	/**
	 * @var CalibrationStartTime
	 * @brief Represents the starting time of a calibration process.
	 *
	 * This variable is used to store the timestamp indicating when a calibration
	 * operation begins. It is typically measured in seconds or another relevant
	 * time unit and functions as a reference point for tracking the duration
	 * or progress of the calibration procedure.
	 *
	 * @details CalibrationStartTime is essential for systems that require precise
	 * synchronization or monitoring of calibration events. It provides a time
	 * reference that can be used to evaluate performance, validate timing,
	 * or manage system states during the calibration process.
	 */
	double CalibrationStartTime;
	/**
	 * @variable CalibrationDuration
	 * @brief Specifies the duration required for a calibration process in the system.
	 *
	 * The CalibrationDuration variable represents the amount of time, in seconds,
	 * allocated for completing the calibration procedure of a specific component or
	 * system. This value can be used to control timing and ensure proper operation
	 * during the calibration phase.
	 *
	 * @details CalibrationDuration plays a crucial role in determining the time
	 * limits for calibration workflows. It may be configured based on the requirements
	 * of the specific hardware or software being calibrated. Proper calibration duration
	 * is essential to achieve accurate results and optimal performance.
	 */
	float CalibrationDuration;
	/**
	 * @class AccumulatedGyro
	 * @brief Represents the accumulated gyroscopic sensor data.
	 *
	 * The AccumulatedGyro variable is used to store the cumulative measurements
	 * from a gyroscope over a period of time. Gyroscopic data typically includes
	 * angular velocity measurements along the X, Y, and Z axes, allowing for
	 * tracking of rotational motion.
	 *
	 * This variable is commonly used in applications requiring precise angular
	 * motion tracking or orientation changes, such as in controllers, VR/AR
	 * systems, or robotics.
	 *
	 * @details The data stored in AccumulatedGyro may include the sum of angular
	 * velocities sampled periodically, providing an aggregate measure of
	 * rotational movement. Proper handling of noise and sensor calibration is
	 * recommended to ensure accuracy when interpreting this data.
	 */
	FVector AccumulatedGyro;
	/**
	 * @class AccumulatedAccel
	 * @brief Represents the total accumulated acceleration vector.
	 *
	 * AccumulatedAccel is a variable intended to store the cumulative acceleration
	 * values detected over time in the form of a 3D vector. This data is typically
	 * used to track movement or behavior in applications requiring motion detection
	 * or spatial calculations.
	 *
	 * @details The variable holds acceleration data along the X, Y, and Z axes,
	 * aggregated over a period. It can be utilized in scenarios such as
	 * gesture recognition, motion analysis, or input handling in systems that
	 * rely on accelerometer-based data or similar sensors.
	 */
	FVector AccumulatedAccel;
	/**
	 * @brief Specifies the number of calibration samples to be collected.
	 *
	 * CalibrationSampleCount is an integer variable that determines the quantity of
	 * samples required for calibration in a given process or system. Increasing or
	 * decreasing this value directly impacts the precision and accuracy of the calibration
	 * process, as more samples generally provide more statistically significant data,
	 * while fewer samples may reduce processing time.
	 *
	 * @details This variable is utilized in scenarios where data consistency, error
	 * adjustment, or parameter tuning is needed for optimal functionality. It plays
	 * a critical role in applications involving sensors, devices, or systems requiring
	 * initialization or recalibration during operation.
	 */
	int32 CalibrationSampleCount;
	/**
	 * @variable AudioVibrationSequence
	 * @brief Represents the identifier for a sequence of audio-guided vibrations.
	 *
	 * This variable is typically used to define or handle a specific pattern of vibrations
	 * that correspond to audio feedback. It serves as a reference to a predefined or dynamically
	 * generated vibration sequence that can synchronize with audio events for immersive experiences.
	 *
	 * @details The value stored in this variable is used to index or differentiate between multiple
	 * vibration sequences. It allows systems to execute specific vibration patterns that align
	 * with corresponding audio playback, enhancing features such as haptic feedback in interactive
	 * environments or devices.
	 */
	uint8 AudioVibrationSequence;
	/**
	 * @brief Represents the context of a Human Interface Device (HID) used by DualSense controllers.
	 *
	 * This variable holds the FDeviceContext structure, which encapsulates the necessary state and
	 * configuration details for managing the connection, communication, and interaction with a
	 * specific DualSense gamepad device. It is primarily used within the UDualSenseLibrary class for
	 * initialization, input handling, and managing device-specific settings.
	 */
	FDeviceContext HIDDeviceContexts;
	/**
	 * @variable GyroBaseline
	 * @brief Represents the baseline gyroscope values for calibration or adjustment.
	 *
	 * The GyroBaseline vector is used to store the initial or default calibration values
	 * of the gyroscope sensor. These baseline values can be used to correct or offset
	 * the raw gyroscope data to account for systematic errors or biases in measurements.
	 *
	 * @details This variable typically holds three-dimensional vector data, representing
	 * the x, y, and z axes of the gyroscope readings. By subtracting or adjusting against
	 * these baseline values, the system can improve the accuracy of motion detection,
	 * ensuring that small deviations or imperfections in the gyroscope's output are compensated for.
	 */
	FVector GyroBaseline;
	/**
	 * @variable AccelBaseline
	 * @brief Represents the baseline accelerometer values for calibration or reference purposes.
	 *
	 * The AccelBaseline variable is used to store the baseline or default accelerometer readings
	 * that can serve as a reference point for motion detection or comparison. It is typically
	 * initialized during a calibration phase and helps in determining deviations or changes in
	 * the accelerometer data during device movement.
	 *
	 * @details This variable generally consists of three components corresponding to the x, y,
	 * and z axes of acceleration. It is useful in systems that involve sensor input for motion
	 * tracking, providing a stable reference to identify movement patterns or biases in the
	 * accelerometer measurements.
	 */
	FVector AccelBaseline;
	/**
	 * @class FusedOrientation
	 * @brief Represents the fused orientation in quaternion format.
	 *
	 * The FusedOrientation variable is used to store the orientation data
	 * expressed as a quaternion. It is typically derived by integrating data
	 * from multiple sources, such as gyroscope, accelerometer, or magnetometer.
	 *
	 * This value is intended for applications that require precise orientation
	 * tracking, such as virtual reality, augmented reality, or motion-based input systems.
	 *
	 * @details The quaternion representation allows for smooth interpolation
	 * and avoidance of issues like gimbal lock, making it ideal for 3D rotational data.
	 */
	FQuat FusedOrientation;
	/**
	 * @class FSensorBounds
	 * @brief Represents the boundaries or limits of a sensor's detectable range.
	 *
	 * The FSensorBounds class is utilized to define and handle the spatial or operational
	 * limits of a sensor within a system. This can include constraints such as minimum and
	 * maximum values that the sensor can register or operate within, which are essential for
	 * validating and processing sensor data.
	 *
	 * @details This class can be especially useful in applications where sensors are required
	 * to operate within strict parameters, ensuring data integrity and preventing erroneous
	 * readings. By defining these bounds, it assists in managing sensor interactions and maintaining
	 * proper system functionality.
	 */
	FSensorBounds Bounds;
};
