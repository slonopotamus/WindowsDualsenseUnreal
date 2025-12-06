// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "API/SonyGamepadProxyHelpers.h"
#include "Core/Types/Structs/Context/DeviceContext.h"
#include "Helpers/ValidateHelpers.h"

namespace FDualSenseTriggerComposer
{
	/**
	 * Disables the trigger functionality for the specified hand or hands on the provided device context.
	 *
	 * @param Context A pointer to the device context which holds the trigger state to be modified.
	 * @param Hand An enumeration specifying which hand's trigger functionality to disable (Left, Right, or AnyHand).
	 */
	inline void Off(FDeviceContext* Context, const EGamepadHand& Hand)
	{
		if (Hand == EGamepadHand::Left || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.LeftTrigger.Mode = 0x0;
		}

		if (Hand == EGamepadHand::Right || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.RightTrigger.Mode = 0x0;
		}
	}

	/**
	 * Configures resistance for the trigger functionality on the specified hand or hands in the given device context.
	 *
	 * @param Context A pointer to the device context that holds the trigger settings to be modified.
	 * @param StartZones The starting position of the resistance zone for the trigger.
	 * @param Strength The level of resistance to be applied within the defined zone.
	 * @param Hand An enumeration specifying which hand's trigger functionality to configure (Left, Right, or AnyHand).
	 */
	inline void Resistance(FDeviceContext* Context, uint8 StartZones, uint8 Strength, const EGamepadHand& Hand)
	{
		if (Hand == EGamepadHand::Left || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.LeftTrigger.Mode = 0x01;
			Context->Output.LeftTrigger.Strengths.Compose[0] = StartZones;
			Context->Output.LeftTrigger.Strengths.Compose[1] = Strength;
		}
		if (Hand == EGamepadHand::Right || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.RightTrigger.Mode = 0x01;
			Context->Output.RightTrigger.Strengths.Compose[0] = StartZones;
			Context->Output.RightTrigger.Strengths.Compose[1] = Strength;
		}
	}

	/**
	 * Configures the trigger functionality on the specified hand or hands to emulate a GameCube-style resistance.
	 *
	 * @param Context A pointer to the device context that holds the trigger settings to be modified.
	 * @param Hand An enumeration specifying which hand's trigger functionality to configure (Left, Right, or AnyHand).
	 */
	inline void GameCube(FDeviceContext* Context, const EGamepadHand& Hand)
	{
		if (Hand == EGamepadHand::Left || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.LeftTrigger.Mode = 0x02;
			Context->Output.LeftTrigger.Strengths.Compose[0] = 0x90;
			Context->Output.LeftTrigger.Strengths.Compose[1] = 0x0a;
			Context->Output.LeftTrigger.Strengths.Compose[2] = 0xff;
		}

		if (Hand == EGamepadHand::Right || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.RightTrigger.Mode = 0x02;
			Context->Output.RightTrigger.Strengths.Compose[0] = 0x90;
			Context->Output.RightTrigger.Strengths.Compose[1] = 0x0a;
			Context->Output.RightTrigger.Strengths.Compose[2] = 0xff;
		}
	}

	inline void Bow22(FDeviceContext* Context, uint8 StartZone, uint8 SnapBack, const EGamepadHand& Hand)
	{
		if (Hand == EGamepadHand::Left || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.LeftTrigger.Mode = 0x22;
			Context->Output.LeftTrigger.Strengths.Compose[0] = StartZone;
			Context->Output.LeftTrigger.Strengths.Compose[1] = 0x01;
			Context->Output.LeftTrigger.Strengths.Compose[2] = SnapBack;
		}

		if (Hand == EGamepadHand::Right || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.RightTrigger.Mode = 0x22;
			Context->Output.RightTrigger.Strengths.Compose[0] = StartZone;
			Context->Output.RightTrigger.Strengths.Compose[1] = 0x01;
			Context->Output.RightTrigger.Strengths.Compose[2] = SnapBack;
		}
	}

	inline void Galloping23(FDeviceContext* Context, uint8 StartPosition, uint8 EndPosition, uint8 FirstFoot, uint8 SecondFoot,
	                        uint8 Frequency, const EGamepadHand& Hand)
	{
		const uint8 FirstFootNib = static_cast<uint8>(FMath::Clamp(FMath::RoundToInt((FirstFoot / 8.0f) * 15.0f), 1, 15));
		const uint8 SecondFootNib = static_cast<uint8>(FMath::Clamp(FMath::RoundToInt((SecondFoot / 8.0f) * 15.0f), 1, 15));
		const uint16 PositionMask = (1 << StartPosition) | (1 << EndPosition);
		if (Hand == EGamepadHand::Left || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.LeftTrigger.Mode = 0x23;
			Context->Output.LeftTrigger.Strengths.Compose[0] = PositionMask & 0xFF;
			Context->Output.LeftTrigger.Strengths.Compose[1] = (PositionMask >> 8) & 0xFF;
			Context->Output.LeftTrigger.Strengths.Compose[2] = ((FirstFootNib & 0x0F) << 4) | (SecondFootNib & 0x0F);
			Context->Output.LeftTrigger.Strengths.Compose[3] = Frequency;
		}

		if (Hand == EGamepadHand::Right || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.RightTrigger.Mode = 0x23;
			Context->Output.RightTrigger.Strengths.Compose[0] = PositionMask & 0xFF;
			Context->Output.RightTrigger.Strengths.Compose[1] = (PositionMask >> 8) & 0xFF;
			Context->Output.RightTrigger.Strengths.Compose[2] = ((FirstFootNib & 0x0F) << 4) | (SecondFootNib & 0x0F);
			Context->Output.RightTrigger.Strengths.Compose[3] = Frequency;
		}
	}

	inline void Weapon25(FDeviceContext* Context, uint8 StartZone, uint8 Amplitude, uint8 Behavior, uint8 Trigger, const EGamepadHand& Hand)
	{
		if (Hand == EGamepadHand::Left || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.LeftTrigger.Mode = 0x25;
			Context->Output.LeftTrigger.Strengths.Compose[0] = StartZone << 4 | (Amplitude & 0x0F);
			Context->Output.LeftTrigger.Strengths.Compose[1] = Behavior;
			Context->Output.LeftTrigger.Strengths.Compose[2] = Trigger & 0x0F;
		}

		if (Hand == EGamepadHand::Right || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.RightTrigger.Mode = 0x25;
			Context->Output.RightTrigger.Strengths.Compose[0] = StartZone << 4 | (Amplitude & 0x0F);
			Context->Output.RightTrigger.Strengths.Compose[1] = Behavior;
			Context->Output.RightTrigger.Strengths.Compose[2] = Trigger & 0x0F;
		}
	}

	inline void MachineGun26(FDeviceContext* Context, uint8 StartZone, uint8 Behavior, uint8 Amplitude, uint8 Frequency, const EGamepadHand& Hand)
	{
		if (Hand == EGamepadHand::Left || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.LeftTrigger.Mode = 0x26;
			Context->Output.LeftTrigger.Strengths.Compose[0] = 0xf8;
			Context->Output.LeftTrigger.Strengths.Compose[1] = Behavior > 0 ? 0x03 : 0x00;
			Context->Output.LeftTrigger.Strengths.Compose[2] = 0x00;
			Context->Output.LeftTrigger.Strengths.Compose[3] = 0x00;
			Context->Output.LeftTrigger.Strengths.Compose[4] = Amplitude == 1 ? 0x8F : 0x8a;
			Context->Output.LeftTrigger.Strengths.Compose[5] = Amplitude == 2 ? 0x3F : 0x1F;
			Context->Output.LeftTrigger.Strengths.Compose[9] = Frequency;
		}

		if (Hand == EGamepadHand::Right || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.RightTrigger.Mode = 0x26;
			Context->Output.RightTrigger.Strengths.Compose[0] = 0xf8;
			Context->Output.RightTrigger.Strengths.Compose[1] = Behavior > 0 ? 0x03 : 0x00;
			Context->Output.RightTrigger.Strengths.Compose[2] = 0x00;
			Context->Output.RightTrigger.Strengths.Compose[3] = 0x00;
			Context->Output.RightTrigger.Strengths.Compose[4] = Amplitude == 1 ? 0x8F : 0x8a;
			Context->Output.RightTrigger.Strengths.Compose[5] = Amplitude == 2 ? 0x3F : 0x1F;
			Context->Output.RightTrigger.Strengths.Compose[9] = Frequency;
		}
	}

	inline void Machine27(FDeviceContext* Context, uint8 StartZone, uint8 BehaviorFlag, uint8 Force, uint8 Amplitude, uint8 Period, uint8 Frequency, const EGamepadHand& Hand)
	{
		if (Hand == EGamepadHand::Left || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.LeftTrigger.Mode = 0x27;
			Context->Output.LeftTrigger.Strengths.Compose[0] = StartZone;
			Context->Output.LeftTrigger.Strengths.Compose[1] = BehaviorFlag > 0 ? 0x02 : 0x01;
			Context->Output.LeftTrigger.Strengths.Compose[2] = Force << 4 | (Amplitude & 0x0F);
			;
			Context->Output.LeftTrigger.Strengths.Compose[3] = Period;
			Context->Output.LeftTrigger.Strengths.Compose[4] = Frequency;
		}

		if (Hand == EGamepadHand::Right || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.RightTrigger.Mode = 0x27;
			Context->Output.RightTrigger.Strengths.Compose[0] = StartZone;
			Context->Output.RightTrigger.Strengths.Compose[1] = BehaviorFlag > 0 ? 0x02 : 0x00;
			Context->Output.RightTrigger.Strengths.Compose[2] = Force << 4 | (Amplitude & 0x0F);
			Context->Output.RightTrigger.Strengths.Compose[3] = Period;
			Context->Output.RightTrigger.Strengths.Compose[4] = Frequency;
		}
	}

	inline void CustomTrigger(FDeviceContext* Context, const EGamepadHand& Hand, const TArray<FString>& HexBytes)
	{
		uint8 Bytes[10] = {0};
		for (int32 i = 0; i < 10; ++i)
		{
			uint8 B = 0;
			if (!FValidateHelpers::ParseHexByte_Local(HexBytes[i], B))
			{
				UE_LOG(LogDualSense, Warning, TEXT("CustomTrigger: invalid hex token at index %d: '%s'"), i, *HexBytes[i]);
				return;
			}
			Bytes[i] = B;
		}

		switch (Bytes[0])
		{
			case 0x01:
			case 0x02:
			case 0x21:
			case 0x22:
			case 0x23:
			case 0x25:
			case 0x26:
			case 0x27: break;
			default: return;
		}

		if (Hand == EGamepadHand::Left || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.LeftTrigger.Mode = 0xFF;
			FMemory::Memcpy(Context->Output.LeftTrigger.Strengths.Compose, Bytes, 10);
		}

		if (Hand == EGamepadHand::Right || Hand == EGamepadHand::AnyHand)
		{
			Context->Output.RightTrigger.Mode = 0xFF;
			FMemory::Memcpy(Context->Output.RightTrigger.Strengths.Compose, Bytes, 10);
		}
	}

} // namespace FDualSenseTriggerComposer
