// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
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
 * @class FAccelerometer
 * @brief Represents an accelerometer component that tracks and provides data related to acceleration.
 *
 * The FAccelerometer class is responsible for handling acceleration readings
 * and providing utility functions to process and retrieve those readings
 * meaningfully. This class can be used to monitor motion in three-dimensional
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
	 * @brief Updates the input state from the gamepad and communicates it to the input system.
	 *
	 * This method processes data received from the gamepad, such as button presses,
	 * joystick movements, and other device-specific inputs. It then transmits this data
	 * to the application via the provided input message handler.
	 *
	 * @param InMessageHandler A shared reference to the message handler used for communicating input events to the application.
	 * @param UserId The unique identifier of the user associated with this input device.
	 * @param InputDeviceId The identifier for the specific input device providing the input.
	 * @param Delta The elapsed time since the last input update, used to calculate input events over time.
	 */
	virtual void UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, float Delta) override {}
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
	 * Sets the lightbar color and associated timing parameters on the gamepad.
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
	 * Initiates the calibration process for the motion sensor on the gamepad.
	 *
	 * @param Duration The duration, in seconds, for which the calibration process should run.
	 * @param DeadZone The threshold value to be used for filtering out small movements.
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
	 * @brief Sets the vibration feedback of the gamepad.
	 *
	 * This method overrides the base implementation to configure the vibration
	 * feedback intensity for the gamepad based on the provided force feedback values.
	 *
	 * @param Values The force feedback values containing the intensity levels for vibration.
	 */
	virtual void SetVibration(const FForceFeedbackValues& Values) override {}

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

	/**
	 * @brief Retrieves a mutable device context associated with the object.
	 *
	 * This method provides access to the device context in a mutable form, allowing modifications to the underlying
	 * hardware interface descriptor (HID) data specific to the device.
	 *
	 * @return A pointer to the mutable FDeviceContext object corresponding to the device's HID context.
	 */
	virtual FDeviceContext* GetMutableDeviceContext() override { return &HIDDeviceContexts; }

	SonyGamepadAbstract()
	    : bEnableTouch(false)
	    , bWasTouch1Down(false)
	    , bWasTouch2Down(false)
	    , bHasPhoneConnected(false)
	    , BatteryLevel(0)
	    , bEnableAccelerometerAndGyroscope(false)
	    , bHasMotionSensorBaseline(false)
	    , bIsCalibrating(false)
	    , CalibrationStartTime(0)
	    , CalibrationDuration(0)
	    , CalibrationSampleCount(0)
	{}

protected:
	[[nodiscard]] bool IsEnableTouch() const { return bEnableTouch; }
	[[nodiscard]] bool IsWasTouch1Down() const { return bWasTouch1Down; }
	void SetWasTouch1Down(bool WasTouch1Down) { this->bWasTouch1Down = WasTouch1Down; }
	[[nodiscard]] bool IsWasTouch2Down() const { return bWasTouch2Down; }
	void SetWasTouch2Down(bool WasTouch2Down) { this->bWasTouch2Down = WasTouch2Down; }
	[[nodiscard]] bool IsHasPhoneConnected() const { return bHasPhoneConnected; }
	void SetHasPhoneConnected(bool HasPhoneConnected) { this->bHasPhoneConnected = HasPhoneConnected; }
	[[nodiscard]] float GetSensorsDeadZone() const { return SensorsDeadZone; }
	void SetSensorsDeadZone(float DeadZone) { this->SensorsDeadZone = FMath::Clamp(DeadZone, 0.0f, 1.f); }
	[[nodiscard]] float GetAnalogDeadZone() const { return AnalogDeadZone; }
	void SetAnalogDeadZone(float DeadZone) { this->AnalogDeadZone = DeadZone; }
	[[nodiscard]] bool IsEnableAccelerometerAndGyroscope() const { return bEnableAccelerometerAndGyroscope; }
	[[nodiscard]] bool IsResetGyroscope() const { return bIsResetGyroscope; }
	void SetIsResetGyroscope(bool ResetGyroscope) { this->bIsResetGyroscope = ResetGyroscope; }
	[[nodiscard]] bool IsHasMotionSensorBaseline() const { return bHasMotionSensorBaseline; }
	void SetHasMotionSensorBaseline(bool MotionSensorBaseline) { this->bHasMotionSensorBaseline = MotionSensorBaseline; }
	[[nodiscard]] bool IsCalibrating() const { return bIsCalibrating; }
	void SetIsCalibrating(bool Calibrating) { this->bIsCalibrating = Calibrating; }
	[[nodiscard]] double GetCalibrationStartTime() const { return CalibrationStartTime; }
	void SetCalibrationStartTime(double CalibrationStart) { this->CalibrationStartTime = CalibrationStart; }
	[[nodiscard]] float GetCalibrationDuration() const { return CalibrationDuration; }
	void SetCalibrationDuration(float Duration) { this->CalibrationDuration = Duration; }
	[[nodiscard]] FVector GetAccumulatedGyro() const { return AccumulatedGyro; }
	void SetAccumulatedGyro(const FVector& AccGyro) { this->AccumulatedGyro = AccGyro; }
	[[nodiscard]] FVector GetAccumulatedAccel() const { return AccumulatedAccel; }
	void SetAccumulatedAccel(const FVector& AccAccel) { this->AccumulatedAccel = AccAccel; }
	void SetCalibrationSampleCount(int32 CalibrationCount) { this->CalibrationSampleCount = FMath::Clamp(CalibrationCount, 1.0f, 10.0f); }
	[[nodiscard]] FVector GetGyroBaseline() const { return GyroBaseline; }
	void SetGyroBaseline(const FVector& Baseline) { this->GyroBaseline = Baseline; }
	[[nodiscard]] FVector GetAccelBaseline() const { return AccelBaseline; }
	void SetAccelBaseline(const FVector& Baseline) { this->AccelBaseline = Baseline; }
	[[nodiscard]] FQuat GetFusedOrientation() const { return FusedOrientation; }
	void SetFusedOrientation(const FQuat& Orientation) { this->FusedOrientation = Orientation; }
	[[nodiscard]] FSensorBounds GetBounds() const { return Bounds; }
	void SetBounds(const FSensorBounds& Bnds) { this->Bounds = Bnds; }
	/**
	 * @brief Sets the battery level for the gamepad.
	 *
	 * This method assigns the specified battery level to the gamepad, ensuring
	 * that the value does not exceed the maximum allowable limit of 100.
	 *
	 * @param Value The battery level as a percentage, where 0 represents an empty battery
	 *              and 100 represents a fully charged battery.
	 */
	void SetBatteryLevel(float Value);
	/**
	 * @brief Increments the calibration sample count.
	 *
	 * This method increases the internal counter used for tracking the number
	 * of calibration samples collected. It is typically utilized during
	 * calibration procedures to ensure accurate device adjustments based
	 * on the sample data.
	 */
	void IncrementCalibrationSampleCount() { CalibrationSampleCount++; }
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
	virtual void CheckButtonInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, const FName ButtonName, const bool IsButtonPressed);

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
