// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

/**
 * @struct FInputContext
 * @brief Represents a structure that captures the current input state of various analog inputs, motion sensors,
 *        and button states from a controller or input device.
 *
 * This structure is used to encapsulate the state of inputs such as analog sticks, triggers, motion sensors,
 * and digital button presses for a given frame or context.
 *
 * Members:
 *
 * ### Analog Inputs
 * - LeftAnalog: 2D vector representing the position of the left analog stick.
 * - RightAnalog: 2D vector representing the position of the right analog stick.
 * - LeftTriggerAnalog: Float value representing the analog input of the left trigger.
 * - RightTriggerAnalog: Float value representing the analog input of the right trigger.
 *
 * ### Motion Sensor Data
 * - Gyroscope: 3D vector representing the angular velocity measured by the gyroscope.
 * - Accelerometer: 3D vector representing the linear acceleration detected by the accelerometer.
 * - Gravity: 3D vector representing the gravity vector.
 * - Tilt: 3D vector representing the tilt or orientation of the device.
 *
 * ### Standard Buttons
 * - bCross: Indicates if the "Cross" button is pressed.
 * - bSquared: Indicates if the "Square" button is pressed.
 * - bTriangle: Indicates if the "Triangle" button is pressed.
 * - bCircle: Indicates if the "Circle" button is pressed.
 * - bDpadUp: Indicates if the D-pad "Up" button is pressed.
 * - bDpadDown: Indicates if the D-pad "Down" button is pressed.
 * - bDpadLeft: Indicates if the D-pad "Left" button is pressed.
 * - bDpadRight: Indicates if the D-pad "Right" button is pressed.
 *
 * ### Special Buttons
 * - bLeftTrigger: Indicates if the left trigger button is pressed.
 * - bRightTrigger: Indicates if the right trigger button is pressed.
 * - bLeftShoulder: Indicates if the left shoulder button is pressed.
 * - bRightShoulder: Indicates if the right shoulder button is pressed.
 * - bLeftStick: Indicates if the left stick button is pressed.
 * - bRightStick: Indicates if the right stick button is pressed.
 * - bPSButton: Indicates if the PlayStation/PS button is pressed.
 * - bShare: Indicates if the "Share" button is pressed.
 * - bStart: Indicates if the "Start" button is pressed.
 * - bTouch: Indicates if the touch input is active.
 * - bMute: Indicates if the "Mute" button is pressed.
 */
struct FInputContext
{
	// temporary
	float AnalogDeadZone = 0.09f;

	// Analogs
	FVector2D LeftAnalog = FVector2D::ZeroVector;
	FVector2D RightAnalog = FVector2D::ZeroVector;
	float LeftTriggerAnalog = 0.0f;
	float RightTriggerAnalog = 0.0f;

	FVector Gyroscope = FVector::ZeroVector;
	FVector Accelerometer = FVector::ZeroVector;
	FVector Gravity = FVector::ZeroVector;
	FVector Tilt = FVector::ZeroVector;

	// touch
	FVector2d TouchRadius = FVector2D::ZeroVector;
	FVector2D TouchPosition = FVector2D::ZeroVector;
	FVector2D TouchStartPosition = FVector2D::ZeroVector;

	int TouchIdOne;
	bool TouchDownOne;
	FVector2d TouchPositionOne = FVector2D::ZeroVector;
	FVector2d TouchLastPositionOne = FVector2D::ZeroVector;

	int TouchIdTwo;
	bool TouchDownTwo;
	FVector2d TouchPositionTwo = FVector2D::ZeroVector;
	FVector2d TouchLastPositionTwo = FVector2D::ZeroVector;

	// touch events
	FVector2D ZoomVector = FVector2D::ZeroVector;
	FVector2D SwipeVector = FVector2D::ZeroVector;
	FVector2D ScrollVector = FVector2D::ZeroVector;

	bool bWasTouchSwipe = false;
	bool bWasTouchZoom = false;
	bool bWasTouchScroll = false;
	bool bWasTouchDownOne = false;
	bool bWasTouchDownTwo = false;
	double CurrentTime = FPlatformTime::Seconds();
	double TouchStartTime;

	// Buttons
	bool bCross;
	bool bSquare;
	bool bTriangle;
	bool bCircle;
	bool bDpadUp;
	bool bDpadDown;
	bool bDpadLeft;
	bool bDpadRight;

	// Special Buttons
	bool bLeftTriggerThreshold;
	bool bRightTriggerThreshold;
	bool bLeftShoulder;
	bool bRightShoulder;
	bool bLeftStick;
	bool bRightStick;
	bool bPSButton;
	bool bShare;
	bool bStart;
	bool bTouch;
	bool bMute;
	bool bHasPhoneConnected;

	// Edge
	bool bFn1;
	bool bFn2;
	bool bPaddleLeft;
	bool bPaddleRight;

	float BatteryLevel;

public:
};
