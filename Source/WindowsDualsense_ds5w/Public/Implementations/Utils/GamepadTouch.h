// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#define DS_TOUCHPAD_WIDTH 1920
#define DS_TOUCHPAD_HEIGHT 1080

#include "Core/Types/Structs/Context/InputContext.h"

namespace FGamepadTouch
{

	inline void Swipe(FInputContext* Input)
	{
		if (Input->TouchDownOne && !Input->bWasTouchDownOne)
		{
			Input->TouchStartPosition = Input->TouchPositionOne;
			Input->TouchStartTime = Input->CurrentTime;
		}
		else if (!Input->TouchDownOne && Input->bWasTouchDownOne)
		{
			float Duration = static_cast<float>(Input->CurrentTime - Input->TouchStartTime);
			float Distance = FVector2D::Distance(Input->TouchPositionOne, Input->TouchStartPosition);
			if (Duration < 0.25f && Distance > 100.0f)
			{
				Input->SwipeVector = Input->TouchPositionOne - Input->TouchStartPosition;
				Input->bWasTouchSwipe = true;
			}
		}
	}

	inline void Scroll(FInputContext* Input)
	{
		const FVector2D Delta1 = Input->TouchPositionOne - Input->TouchLastPositionOne;
		const FVector2D Delta2 = Input->TouchPositionTwo - Input->TouchLastPositionTwo;
		if ((Delta1 | Delta2) > 0.0f)
		{
			if (const FVector2D AvgDelta = (Delta1 + Delta2) * 0.5f; AvgDelta.SizeSquared() > 25.0f)
			{
				Input->ScrollVector = AvgDelta;
				Input->bWasTouchScroll = true;
			}
		}
	}

	inline void Magnify(FInputContext* Input)
	{
		if (Input->TouchDownOne && Input->TouchDownTwo && Input->bWasTouchDownOne && Input->bWasTouchDownTwo)
		{
			float CurrentDist = FVector2D::Distance(Input->TouchPositionOne, Input->TouchPositionTwo);
			float PrevDist = FVector2D::Distance(Input->TouchLastPositionOne, Input->TouchLastPositionTwo);
			float Delta = CurrentDist - PrevDist;
			if (FMath::Abs(CurrentDist - PrevDist) > 5.0f)
			{
				Input->ZoomVector = FVector2D(Delta, 0.0f);
				Input->bWasTouchZoom = true;
			}
		}
	}

	inline void Reset(FInputContext* Input)
	{
		Input->bWasTouchZoom = false;
		Input->bWasTouchSwipe = false;
		Input->bWasTouchScroll = false;
		Input->ZoomVector = FVector2D::ZeroVector;
		Input->SwipeVector = FVector2D::ZeroVector;
		Input->ScrollVector = FVector2D::ZeroVector;

		if (Input->TouchRadius == FVector2D::ZeroVector)
		{
			Input->TouchRadius = FVector2D(DS_TOUCHPAD_WIDTH, DS_TOUCHPAD_HEIGHT);
		}
	}

	inline bool IsTouching(const FVector2D& TouchPosition, const FVector2D& TouchRadius)
	{
		return TouchPosition.X >= 0 && TouchPosition.X <= TouchRadius.X && TouchPosition.Y >= 0 && TouchPosition.Y <= TouchRadius.Y;
	}

	inline void ProcessTouch(const unsigned char* HIDInput, FInputContext* Input)
	{
		Reset(Input);

		const int32 Touchpad1Raw = *reinterpret_cast<const int32*>(&HIDInput[0x20]);

		Input->TouchPositionOne.Y = (Touchpad1Raw & 0xFFF00000) >> 20;
		Input->TouchPositionOne.X = (Touchpad1Raw & 0x000FFF00) >> 8;

		Input->TouchIdOne = (Touchpad1Raw & 127) % 10;
		Input->TouchDownOne = (Touchpad1Raw & (1 << 7)) == 0;

		if (Input->TouchDownOne && !IsTouching(Input->TouchPositionOne, Input->TouchRadius))
		{
			Input->TouchDownOne = false;
		}

		const int32 Touchpad2Raw = *reinterpret_cast<const int32*>(&HIDInput[0x24]);
		Input->TouchPositionTwo.Y = (Touchpad2Raw & 0xFFF00000) >> 20;
		Input->TouchPositionTwo.X = (Touchpad2Raw & 0x000FFF00) >> 8;

		Input->TouchIdTwo = (Touchpad2Raw & 127) % 10;
		Input->TouchDownTwo = (Touchpad2Raw & (1 << 7)) == 0;

		if (Input->TouchDownTwo && !IsTouching(Input->TouchPositionTwo, Input->TouchRadius))
		{
			Input->TouchDownTwo = false;
		}

		// Swipe(Input);
		// Scroll(Input);
		// Magnify(Input);
	}

} // namespace FGamepadTouch
