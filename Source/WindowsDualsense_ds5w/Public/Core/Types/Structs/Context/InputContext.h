// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

enum class ETouchInteraction : uint8
{
	None,
	Swipe,
	Zoom,
	Scroll
};

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
	int32 TouchId = 0;
	int32 TouchFingerCount = 0;
    
	// Scroll (Flick)
	double CurrentTime = 0.0;
	double TouchStartTime = 0.0;
	bool bIsTouching = false;
	bool bWasTouchDown = false;

	uint8 DirectionRaw = 0.0f;
	float DirectionAngle = 0.0f;

	FVector2D P1_Current = FVector2D::ZeroVector;
	FVector2D P1_Last = FVector2D::ZeroVector;
    
	FVector2D P2_Current = FVector2D::ZeroVector;
	FVector2D P2_Last = FVector2D::ZeroVector;
    
	float ZoomDelta = 0.0f;
	FVector2D SwipeVector = FVector2D::ZeroVector;
	FVector2D ScrollVelocity = FVector2D::ZeroVector;
	
	FVector2d TouchRadius = FVector2D::ZeroVector;
	FVector2d TouchPosition = FVector2D::ZeroVector;
	FVector2d TouchRelative = FVector2D::ZeroVector;
	ETouchInteraction TouchInteraction = ETouchInteraction::None;

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
};
