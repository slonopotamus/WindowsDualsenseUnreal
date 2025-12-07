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
		const FVector2D Delta1 = Input->P1_Current - Input->P1_Last;
		const FVector2D Delta2 = Input->P2_Current - Input->P2_Last;

		if (Input->TouchFingerCount == 1)
		{
			Input->SwipeVector = Delta1;
			
			if (Input->SwipeVector.SizeSquared() > 10.0f)
			{
				Input->TouchInteraction = ETouchInteraction::Swipe;
			}
			return;
		}
		
		if (Input->TouchFingerCount == 2)
		{
			if (const float DotProd = FVector2D::DotProduct(Delta1.GetSafeNormal(), Delta2.GetSafeNormal()); DotProd > 0.7f)
			{
				Input->SwipeVector = (Delta1 + Delta2) * 0.5f;
				Input->TouchInteraction = ETouchInteraction::Swipe;
			}
		}
	}

	inline void Zoom(FInputContext* Input)
	{
		if (Input->TouchFingerCount != 2) return;

		const float DistCurrent = FVector2D::Distance(Input->P1_Current, Input->P2_Current);
		const float DistLast = FVector2D::Distance(Input->P1_Last, Input->P2_Last);

		if (const float Delta = DistCurrent - DistLast; FMath::Abs(Delta) > 1.0f)
		{
			Input->ZoomDelta = Delta;
			Input->TouchInteraction = ETouchInteraction::Zoom;
		}
	}
	
	inline void Scroll(FInputContext* Input)
	{
		if (!Input->bIsTouching && Input->TouchFingerCount == 0)
		{
			if (const double Duration = Input->CurrentTime - Input->TouchStartTime; Duration < 0.25f && Duration > 0.0f)
			{
				const float AngleRad = FMath::DegreesToRadians((Input->DirectionRaw / 255.0f) * 360.0f);
				const FVector2D HardwareDirection(FMath::Cos(AngleRad), FMath::Sin(AngleRad));
				Input->ScrollVelocity = HardwareDirection * Duration;
				Input->TouchInteraction = ETouchInteraction::Scroll;
			}
		}
	}
	
	inline void ProcessGestures(FInputContext* Input)
	{
		Input->TouchInteraction = ETouchInteraction::None;
		if (!Input->bIsTouching)
		{
			Scroll(Input);
			return;
		}
		
		if (Input->TouchFingerCount == 2)
		{
			Zoom(Input);
			if (Input->TouchInteraction == ETouchInteraction::None)
			{
				Swipe(Input);
			}
		}
		else if (Input->TouchFingerCount == 1)
		{
			Swipe(Input);
		}
	}
	
	inline bool IsTouching(const FVector2D& TouchPosition, const FVector2D& TouchRadius)
	{
		return TouchPosition.X >= 0 && TouchPosition.X <= TouchRadius.X && TouchPosition.Y >= 0 && TouchPosition.Y <= TouchRadius.Y;
	}
	
	inline void FingerPositions(const unsigned char* HIDInput, FInputContext* Input)
	{
		const int32 AbsX = ((HIDInput[0x22] & 0x0F) << 8) | HIDInput[0x21];
		const int32 AbsY = (HIDInput[0x23] << 4) | ((HIDInput[0x22] & 0xF0) >> 4);
		Input->TouchPosition = FVector2D(AbsX, AbsY);

		const int32 AbsRelativeX = ((HIDInput[0x27] & 0x0F) << 8) | HIDInput[0x25];
		const int32 AbsRelativeY = (HIDInput[0x26] << 4) | ((HIDInput[0x27] & 0xF0) >> 4);
		Input->TouchRelative = FVector2D(AbsRelativeX, AbsRelativeY);
		
		Input->P1_Last = Input->P1_Current;
		Input->P1_Current = Input->TouchPosition;
		
		Input->P2_Last = Input->P2_Current;
		Input->P2_Current = Input->TouchRelative;
		
		Input->TouchFingerCount = Input->bIsTouching && (HIDInput[0x24] & 0x80) != 0 ? 2 : 1;
	}

	inline void ProcessTouch(const unsigned char* HIDInput, FInputContext* Input)
	{
		if (Input->TouchRadius == FVector2D::ZeroVector)
		{
			Input->TouchRadius = FVector2D(DS_TOUCHPAD_WIDTH, DS_TOUCHPAD_HEIGHT);
		}
		
		
		Input->TouchId = (HIDInput[0x20] & 0x7F) % 10;
		Input->bIsTouching = (HIDInput[0x20] & 0x80) != 0;
		Input->DirectionRaw = HIDInput[0x28];
		FingerPositions(HIDInput, Input);
		ProcessGestures(Input);
	}

} // namespace FGamepadTouch
