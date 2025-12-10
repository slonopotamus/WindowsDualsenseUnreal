// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#define DS_TOUCHPAD_WIDTH 1920
#define DS_TOUCHPAD_HEIGHT 1080

#include "Core/Types/Structs/Context/InputContext.h"

namespace FGamepadTouch
{
	inline bool IsTouching(const FVector2D& TouchPosition, const FVector2D& TouchRadius)
	{
		return TouchPosition.X >= 0 && TouchPosition.X <= TouchRadius.X && TouchPosition.Y >= 0 && TouchPosition.Y <= TouchRadius.Y;
	}

	inline void ProcessTouch(const unsigned char* HIDInput, FInputContext* Input)
	{
		if (Input->TouchRadius.X == 0.0f || Input->TouchRadius.Y == 0.0f)
		{
			Input->TouchRadius = {DS_TOUCHPAD_WIDTH, DS_TOUCHPAD_HEIGHT};
		}

		Input->TouchId = (HIDInput[0x20] & 0x7F) % 10;
		Input->bIsTouching = (HIDInput[0x20] & 0x80) != 0;
		Input->DirectionRaw = HIDInput[0x28];

		const float AbsX = ((HIDInput[0x22] & 0x0F) << 8) | HIDInput[0x21];
		const float AbsY = (HIDInput[0x23] << 4) | ((HIDInput[0x22] & 0xF0) >> 4);
		Input->TouchPosition = {AbsX, AbsY};

		const float AbsRelativeX = ((HIDInput[0x27] & 0x0F) << 8) | HIDInput[0x25];
		const float AbsRelativeY = (HIDInput[0x26] << 4) | ((HIDInput[0x27] & 0xF0) >> 4);
		Input->TouchRelative = {AbsRelativeX, AbsRelativeY};

		Input->P1_Last = Input->P1_Current;
		Input->P1_Current = Input->TouchPosition;

		Input->P2_Last = Input->P2_Current;
		Input->P2_Current = Input->TouchRelative;

		Input->TouchFingerCount = Input->bIsTouching && (HIDInput[0x24] & 0x80) != 0 ? 2 : 1;
	}

} // namespace FGamepadTouch
