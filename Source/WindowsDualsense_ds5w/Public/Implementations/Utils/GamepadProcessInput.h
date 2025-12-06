// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "Core/Types/Structs/Context/DeviceContext.h"

#define BTN_FN1 0x10
#define BTN_FN2 0x20
#define BTN_PADDLE_LEFT 0x40
#define BTN_PADDLE_RIGHT 0x80

#define BTN_DPAD_UP 0x8
#define BTN_DPAD_DOWN 0x02
#define BTN_DPAD_LEFT 0x01
#define BTN_DPAD_RIGHT 0x04

#define BTN_CROSS 0x20
#define BTN_SQUARE 0x10
#define BTN_CIRCLE 0x40
#define BTN_TRIANGLE 0x80

#define BTN_LEFT_STICK 0x40
#define BTN_RIGHT_STICK 0x80

#define BTN_LEFT_SHOULDER 0x01
#define BTN_RIGHT_SHOULDER 0x02
#define BTN_LEFT_TRIGGER 0x04
#define BTN_RIGHT_TRIGGER 0x08

#define BTN_START 0x20
#define BTN_SELECT 0x10
#define BTN_PAD_BUTTON 0x02
#define BTN_MIC_BUTTON 0x04
#define BTN_PLAYSTATION_LOGO 0x01

namespace FGamepadProcessInput
{
	inline void DualSenseRaw(const unsigned char* HIDInput, FInputContext* Input)
	{
		// Analogs
		const float LeftAnalogX = static_cast<float>(HIDInput[0x00] - 128) / 128.f;
		const float LeftAnalogY = static_cast<float>(HIDInput[0x01] - 128) / -128.f;
		const float RightAnalogX = static_cast<float>(HIDInput[0x02] - 128) / 128.f;
		const float RightAnalogY = static_cast<float>(HIDInput[0x03] - 128) / -128.f;

		const float TriggerL = HIDInput[0x04] / 256.0f;
		const float TriggerR = HIDInput[0x05] / 256.0f;

		uint8_t ButtonsMask = HIDInput[0x07] & 0xF0;
		const bool bCross = ButtonsMask & BTN_CROSS;
		const bool bSquare = ButtonsMask & BTN_SQUARE;
		const bool bCircle = ButtonsMask & BTN_CIRCLE;
		const bool bTriangle = ButtonsMask & BTN_TRIANGLE;

		switch (HIDInput[0x07] & 0x0F)
		{
			case 0x0:
				ButtonsMask |= BTN_DPAD_UP;
				break;
			case 0x4:
				ButtonsMask |= BTN_DPAD_DOWN;
				break;
			case 0x6:
				ButtonsMask |= BTN_DPAD_LEFT;
				break;
			case 0x2:
				ButtonsMask |= BTN_DPAD_RIGHT;
				break;
			case 0x5:
				ButtonsMask |= BTN_DPAD_LEFT | BTN_DPAD_DOWN;
				break;
			case 0x7:
				ButtonsMask |= BTN_DPAD_LEFT | BTN_DPAD_UP;
				break;
			case 0x1:
				ButtonsMask |= BTN_DPAD_RIGHT | BTN_DPAD_UP;
				break;
			case 0x3:
				ButtonsMask |= BTN_DPAD_RIGHT | BTN_DPAD_DOWN;
				break;
			default:;
		}

		const bool bDPadLeft = ButtonsMask & BTN_DPAD_LEFT;
		const bool bDPadDown = ButtonsMask & BTN_DPAD_DOWN;
		const bool bDPadRight = ButtonsMask & BTN_DPAD_RIGHT;
		const bool bDPadUp = ButtonsMask & BTN_DPAD_UP;

		// Shoulders
		const bool bLeftShoulder = HIDInput[0x08] & BTN_LEFT_SHOULDER;
		const bool bRightShoulder = HIDInput[0x08] & BTN_RIGHT_SHOULDER;

		// Push Stick
		const bool PushLeftStick = HIDInput[0x08] & BTN_LEFT_STICK;
		const bool PushRightStick = HIDInput[0x08] & BTN_RIGHT_STICK;

		// Function & Special Actions
		const bool Playstation = HIDInput[0x09] & BTN_PLAYSTATION_LOGO;
		const bool TouchPad = HIDInput[0x09] & BTN_PAD_BUTTON;
		const bool Mic = HIDInput[0x09] & BTN_MIC_BUTTON;
		const bool bFn1 = HIDInput[0x09] & BTN_FN1;
		const bool bFn2 = HIDInput[0x09] & BTN_FN2;
		const bool bPaddleLeft = HIDInput[0x09] & BTN_PADDLE_LEFT;
		const bool bPaddleRight = HIDInput[0x09] & BTN_PADDLE_RIGHT;

		const bool Start = HIDInput[0x08] & BTN_START;
		const bool Select = HIDInput[0x08] & BTN_SELECT;

		const bool bLeftTriggerThreshold = HIDInput[0x08] & BTN_LEFT_TRIGGER;
		const bool bRightTriggerThreshold = HIDInput[0x08] & BTN_RIGHT_TRIGGER;

		Input->bHasPhoneConnected = (HIDInput[0x35] & 0x01);
		Input->BatteryLevel = (((HIDInput[0x34] & 0x0F) / 10.0) * 100);

		auto ApplyDeadZone = [](float Value, float Threshold) -> float {
			if (FMath::Abs(Value) < Threshold)
			{
				return 0.0f;
			}

			const float Sign = FMath::Sign(Value);
			const float Abs = FMath::Abs(Value);
			return Sign * ((Abs - Threshold) / (1.0f - Threshold));
		};

		Input->LeftAnalog.X = ApplyDeadZone(LeftAnalogX, Input->AnalogDeadZone);
		Input->LeftAnalog.Y = ApplyDeadZone(LeftAnalogY, Input->AnalogDeadZone);
		Input->RightAnalog.X = ApplyDeadZone(RightAnalogX, Input->AnalogDeadZone);
		Input->RightAnalog.Y = ApplyDeadZone(RightAnalogY, Input->AnalogDeadZone);

		Input->LeftTriggerAnalog = TriggerL;
		Input->RightTriggerAnalog = TriggerR;
		Input->bCross = bCross;
		Input->bSquare = bSquare;
		Input->bCircle = bCircle;
		Input->bTriangle = bTriangle;
		Input->bDpadUp = bDPadUp;
		Input->bDpadDown = bDPadDown;
		Input->bDpadLeft = bDPadLeft;
		Input->bDpadRight = bDPadRight;
		Input->bLeftShoulder = bLeftShoulder;
		Input->bRightShoulder = bRightShoulder;
		Input->bStart = Start;
		Input->bShare = Select;
		Input->bLeftTriggerThreshold = bLeftTriggerThreshold;
		Input->bRightTriggerThreshold = bRightTriggerThreshold;
		Input->bLeftStick = PushLeftStick;
		Input->bRightStick = PushRightStick;

		Input->bMute = Mic;
		Input->bFn1 = bFn1;
		Input->bFn2 = bFn2;
		Input->bTouch = TouchPad;
		Input->bPSButton = Playstation;
		Input->bPaddleLeft = bPaddleLeft;
		Input->bPaddleRight = bPaddleRight;
	}

	inline void DualShockRaw(const unsigned char* HIDInput, FInputContext* Input)
	{
		// Triggers
		const bool bLeftTriggerThreshold = HIDInput[0x05] & BTN_LEFT_TRIGGER;
		const bool bRightTriggerThreshold = HIDInput[0x05] & BTN_RIGHT_TRIGGER;

		// Triggers Analog 1D
		const float TriggerL = HIDInput[0x07] / 256.0f;
		const float TriggerR = HIDInput[0x08] / 256.0f;

		const float LeftAnalogX = static_cast<float>(HIDInput[0x00] - 128) / 128.f;
		const float LeftAnalogY = static_cast<float>(HIDInput[0x01] - 128) / -128.f;
		const float RightAnalogX = static_cast<float>(HIDInput[0x02] - 128) / 128.f;
		const float RightAnalogY = static_cast<float>(HIDInput[0x03] - 128) / -128.f;

		uint8_t ButtonsMask = HIDInput[0x04] & 0xF0;
		const bool bCross = ButtonsMask & BTN_CROSS;
		const bool bSquare = ButtonsMask & BTN_SQUARE;
		const bool bCircle = ButtonsMask & BTN_CIRCLE;
		const bool bTriangle = ButtonsMask & BTN_TRIANGLE;

		switch (HIDInput[0x04] & 0x0F)
		{
			case 0x0:
				ButtonsMask |= BTN_DPAD_UP;
				break;
			case 0x4:
				ButtonsMask |= BTN_DPAD_DOWN;
				break;
			case 0x6:
				ButtonsMask |= BTN_DPAD_LEFT;
				break;
			case 0x2:
				ButtonsMask |= BTN_DPAD_RIGHT;
				break;
			case 0x5:
				ButtonsMask |= BTN_DPAD_LEFT | BTN_DPAD_DOWN;
				break;
			case 0x7:
				ButtonsMask |= BTN_DPAD_LEFT | BTN_DPAD_UP;
				break;
			case 0x1:
				ButtonsMask |= BTN_DPAD_RIGHT | BTN_DPAD_UP;
				break;
			case 0x3:
				ButtonsMask |= BTN_DPAD_RIGHT | BTN_DPAD_DOWN;
				break;
			default:;
		}
		const bool bDPadLeft = ButtonsMask & BTN_DPAD_LEFT;
		const bool bDPadDown = ButtonsMask & BTN_DPAD_DOWN;
		const bool bDPadRight = ButtonsMask & BTN_DPAD_RIGHT;
		const bool bDPadUp = ButtonsMask & BTN_DPAD_UP;

		// Shoulders
		const bool bLeftShoulder = HIDInput[0x05] & BTN_LEFT_SHOULDER;
		const bool bRightShoulder = HIDInput[0x05] & BTN_RIGHT_SHOULDER;

		// Push Stick
		const bool PushLeftStick = HIDInput[0x05] & BTN_LEFT_STICK;
		const bool PushRightStick = HIDInput[0x05] & BTN_RIGHT_STICK;

		const bool Start = HIDInput[0x05] & BTN_START;
		const bool Select = HIDInput[0x05] & BTN_SELECT;

		auto ApplyDeadZone = [](float Value, float Threshold) -> float {
			if (FMath::Abs(Value) < Threshold)
			{
				return 0.0f;
			}

			const float Sign = FMath::Sign(Value);
			const float Abs = FMath::Abs(Value);
			return Sign * ((Abs - Threshold) / (1.0f - Threshold));
		};

		Input->LeftAnalog.X = ApplyDeadZone(LeftAnalogX, Input->AnalogDeadZone);
		Input->LeftAnalog.Y = ApplyDeadZone(LeftAnalogY, Input->AnalogDeadZone);
		Input->RightAnalog.X = ApplyDeadZone(RightAnalogX, Input->AnalogDeadZone);
		Input->RightAnalog.Y = ApplyDeadZone(RightAnalogY, Input->AnalogDeadZone);

		Input->LeftTriggerAnalog = TriggerL;
		Input->RightTriggerAnalog = TriggerR;
		Input->bCross = bCross;
		Input->bSquare = bSquare;
		Input->bCircle = bCircle;
		Input->bTriangle = bTriangle;
		Input->bDpadUp = bDPadUp;
		Input->bDpadDown = bDPadDown;
		Input->bDpadLeft = bDPadLeft;
		Input->bDpadRight = bDPadRight;
		Input->bLeftShoulder = bLeftShoulder;
		Input->bRightShoulder = bRightShoulder;
		Input->bLeftStick = PushLeftStick;
		Input->bRightStick = PushRightStick;
		Input->bStart = Start;
		Input->bShare = Select;
		Input->bLeftTriggerThreshold = bLeftTriggerThreshold;
		Input->bRightTriggerThreshold = bRightTriggerThreshold;
	}

} // namespace FGamepadProcessInput
