// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "Core/Algorithms/MadgwickAhrs.h"
#include "Core/Interfaces/ISonyGamepad.h"
#include "Implementations/Libraries/DualSense/DualSenseLibrary.h"

/**
 * @class FTouchPoint1
 * @brief Represents a touchpoint in a touch-based input system.
 *
 * The FTouchPoint1 class is designed to encapsulate the properties and behavior
 * of a single touchpoint detected on a touch-sensitive surface. It provides
 * information such as the position, pressure, and state of the touchpoint,
 * which can be used to build touch interactions within an application.
 *
 * This class is typically used in systems that handle multitouch input,
 * providing the data necessary to track individual touchpoints and their
 * specific attributes.
 *
 * @details The class may store information about the position of the touchpoint
 * in screen coordinates, the pressure or force of the touch, and the current state
 * of the touch event (e.g., touch began, moved, or ended). It may be used in
 * conjunction with other similar touchpoint objects for multitouch gesture recognition
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
	 * @return The factorial of the input number. If input  0, returns 1.
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
	 * They'd class can be used in scenarios where unique identification
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
 * The FTouchPoint2 class is used to encapsulate the state and properties of a touchpoint
 * as it interacts with a touch-sensitive interface. Each instance of this class provides
 * detailed information about the current state of the touchpoint, including its position,
 * pressure, and movement. This is typically used in multitouch systems to track and manage
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
	 * @brief Represents a variable 'Y' with an unspecified type and purpose.
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
	 * It is typically used for identification.
	 */
	unsigned char Id;
};


/**
 * @class SonyGamepadAbstract
 * @brief An abstract base class derived from the ISonyGamepad interface.
 *
 * The SonyGamepadAbstract class serves as a foundational class for implementing
 * gamepad functionality specific to Sony devices. It inherits from the ISonyGamepad
 * interface, which provides methods and features for managing gamepad status, lightbar,
 * sensors, touch input, triggers, audio-driven haptics, and more.
 *
 * Classes inheriting from SonyGamepadAbstract are expected to implement the behaviors
 * defined by the ISonyGamepad interface.
 */
class SonyGamepadAbstract : public ISonyGamepad
{

public:
	/**
	 * @brief Shuts down the library and releases associated resources.
	 *
	 * This method is invoked to perform cleanup tasks, ensuring that all resources
	 * allocated by the library are properly released. It overrides the equivalent
	 * method in the ISonyGamepad interface and ensures the integrity of the shutdown
	 * process specific to Sony gamepad implementations.
	 */
	virtual void ShutdownLibrary() override;
	/**
	 * @brief Updates the output state of the gamepad.
	 *
	 * This method is responsible for refreshing or modifying the output-related
	 * functionality of the Sony gamepad. It overrides the UpdateOutput method
	 * defined in the ISonyGamepad interface and provides a customized implementation
	 * specific to derived classes.
	 *
	 * The UpdateOutput functionality may include updating lightbar states,
	 * haptic feedback, audio output, or other gamepad output features based on
	 * the current state or input from the system.
	 */
	virtual void UpdateOutput() override {}
	/**
	 * @brief Updates the input state of the gamepad.
	 *
	 * The UpdateInput method is called to process and refresh the input data
	 * associated with the gamepad. This function is typically invoked on each
	 * frame with the elapsed time (Delta) since the last update, allowing input
	 * state to be updated in real-time.
	 *
	 * @param Delta The time elapsed since the previous frame, in seconds.
	 *
	 * @details This method is meant to be overridden to implement specific
	 * behaviors for handling input updates in derived classes. It ensures that
	 * the gamepad's state is continuously synchronized with the current input
	 * conditions, supporting real-time interaction within the application.
	 */
	virtual void UpdateInput(float Delta) override {}
	/**
	 * Pure virtual function that checks the connection status of the gamepad.
	 *
	 * @return True if the gamepad is connected, false otherwise.
	 */
	virtual bool IsConnected() override;
	/**
	 * Retrieves the type of the device.
	 *
	 * @return The type of the device as an EDeviceType enumeration.
	 */
	virtual EDeviceType GetDeviceType() override;
	/**
	 * Retrieves the connection type of the device.
	 *
	 * @return The connection type of the device as an EDeviceConnection enumeration.
	 */
	virtual EDeviceConnection GetConnectionType() override;
	/**
	 * Retrieves the current battery level of the Sony gamepad.
	 *
	 * @return The battery level as a floating-point value, where the exact range
	 *         and representation are dependent on the implementation. Typically,
	 *         values may range between 0.0 (empty) and 1.0 (full).
	 */
	virtual float GetBattery() const override;
	/**
	 * Stops all currently active operations or actions associated with the interface.
	 * This method must be implemented by any derived class to handle the termination
	 * of all ongoing processes.
	 */
	virtual void ResetLights() override {}
	/**
	 * Sets the lightbar color and associated timing parameters on the gamepad DualShock4.
	 *
	 * @param Color The color to set the lightbar to, represented as an FColor.
	 * @param BrithnessTime The time duration for setting the brightness effect. Defaults to 0.0f.
	 * @param ToggleTime The time interval for toggling the lightbar. Defaults to 0.0f.
	 */
	virtual void SetLightbar(FColor Color, float BrithnessTime = 0.0f, float ToggleTime = 0.0f) override {}

	/**
	 * Sets the LED associated with the player on the Sony gamepad to a specified brightness level.
	 *
	 * @param Led The LED associated with a specific player, represented as an ELedPlayerEnum.
	 * @param Brightness The desired brightness level for the LED, represented as an ELedBrightnessEnum.
	 */
	virtual void SetPlayerLed(ELedPlayerEnum Led, ELedBrightnessEnum Brightness) override {}
	/**
	 * Sets the state of the microphone LED on the gamepad.
	 *
	 * @param Led The desired state of the microphone LED, represented by ELedMicEnum.
	 */
	virtual void SetMicrophoneLed(ELedMicEnum Led) override {}
	/**
	 * Sets the touch state for the device.
	 *
	 * @param bIsTouch A boolean indicating whether touch input is enabled (true) or disabled (false).
	 */
	virtual void EnableTouch(const bool bIsTouch) override;
	/**
	 * Resets the orientation of the gyroscope to its default state.
	 * Typically used to recalibrate the gyroscope sensor.
	 */
	virtual void ResetGyroOrientation() override;
	/**
	 * Enables the motion sensor functionality of the gamepad.
	 *
	 * @param bIsMotionSensor Specifies whether to enable the gyroscope (true) or accelerometer (false) as the motion sensor.
	 */
	virtual void EnableMotionSensor(bool bIsMotionSensor) override;
	/**
	 * @brief Sets the vibration feedback of the gamepad.
	 *
	 * This method overrides the base implementation to configure the vibration
	 * feedback intensity for the gamepad based on the provided force feedback values.
	 *
	 * @param Values The force feedback values containing the intensity levels for vibration.
	 */
	virtual void SetVibration(const FForceFeedbackValues& Values) override {}

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
	 * @class FMadgwickAhrs
	 * @brief Implements the Madgwick filter algorithm for sensor fusion in orientation tracking.
	 *
	 * The FMadgwickAhrs class is designed to process data from an IMU (Inertial Measurement Unit),
	 * applying the Madgwick filter algorithm to estimate orientation (roll, pitch, and yaw).
	 * It fuses accelerometer, gyroscope, and optionally magnetometer data to provide
	 * reliable orientation estimations.
	 *
	 * This class is commonly utilized in applications requiring real-time orientation tracking,
	 * such as motion sensing, virtual reality systems, and robotics.
	 *
	 * @details The algorithm is specifically optimized for low computational overhead while
	 * maintaining accurate results. It is well-suited for embedded systems or devices with
	 * constrained processing resources. Using this class, applications can achieve smooth
	 * and responsive 6-DOF (Degrees of Freedom) motion and orientation tracking.
	 */
	FMadgwickAhrs MadgwickFilter;
	
	SonyGamepadAbstract()
	    : MadgwickFilter(0.8)
	    , bEnableTouch(false)
	    , bWasTouch1Down(false)
	    , bWasTouch2Down(false)
	    , bHasPhoneConnected(false)
	    , BatteryLevel(0)
	    , bEnableAccelerometerAndGyroscope(false)
	    , HIDDeviceContexts()
	{}

protected:
	[[nodiscard]] bool IsEnableTouch() const { return bEnableTouch; }
	[[nodiscard]] bool IsWasTouch1Down() const { return bWasTouch1Down; }
	void SetWasTouch1Down(bool WasTouch1Down) { this->bWasTouch1Down = WasTouch1Down; }
	[[nodiscard]] bool IsWasTouch2Down() const { return bWasTouch2Down; }
	void SetWasTouch2Down(bool WasTouch2Down) { this->bWasTouch2Down = WasTouch2Down; }
	[[nodiscard]] bool IsEnableAccelerometerAndGyroscope() const { return bEnableAccelerometerAndGyroscope; }
	[[nodiscard]] bool IsResetGyroscope() const { return bIsResetGyroscope; }
	void SetIsResetGyroscope(const bool IsResetGyroscope) { this->bIsResetGyroscope = IsResetGyroscope; }
	
	/**
	 * @brief Sets the device contexts for the HID device.
	 *
	 * This method assigns the provided device contexts to the internal HID device contexts,
	 * enabling the configuration or update of the underlying device settings.
	 *
	 * @param DeviceContexts A reference to the FDeviceContext object representing
	 * the new HID device contexts.
	 */
	void SetDeviceContexts(const FDeviceContext& DeviceContexts) { this->HIDDeviceContexts = DeviceContexts; }

private:
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
	 * This variable is useful for identifying changes in the touch state, such as
	 * transitions between touchdown and touch up events, or for implementing
	 * touch-based controls in applications.
	 *
	 * @details This flag is often utilized within systems handling touch input
	 * to determine the prior state of a touchpoint. By comparing its current
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
	 * a role in monitoring touch events, such as detecting when the second touchpoint
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
	bool bHasPhoneConnected;
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
	float BatteryLevel;
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
	 * @brief Represents the context of a Human Interface Device (HID) used by DualSense controllers.
	 *
	 * This variable holds the FDeviceContext structure, which encapsulates the necessary state and
	 * configuration details for managing the connection, communication, and interaction with a
	 * specific DualSense gamepad device. It is primarily used within the UDualSenseLibrary class for
	 * initialization, input handling, and managing device-specific settings.
	 */
	FDeviceContext HIDDeviceContexts;
};
