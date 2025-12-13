// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "GCore/Interfaces/ISonyGamepad.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "Helpers/DualSenseLog.h"
#include "Implementations/Adapters/DeviceRegistry.h"
#include "Types/Enums/EDeviceCommons.h"
#include "Types/Enums/EDeviceConnection.h"

// Forward declarations
class IGamepadTrigger;
class IGamepadAudioHaptics;

static_assert(static_cast<uint8>(EDeviceType::DualSense) == static_cast<uint8>(EDSDeviceType::DualSense), "Enum mismatch: DualSense value differs between Core and API!");
static_assert(static_cast<uint8>(EDeviceType::DualShock4) == static_cast<uint8>(EDSDeviceType::DualShock4), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(EDeviceType::DualSenseEdge) == static_cast<uint8>(EDSDeviceType::DualSenseEdge), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(EDeviceType::DualSenseEdge) == static_cast<uint8>(EDSDeviceType::DualSenseEdge), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(EDeviceConnection::Usb) == static_cast<uint8>(EDSDeviceConnection::Usb), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(EDeviceConnection::Bluetooth) == static_cast<uint8>(EDSDeviceConnection::Bluetooth), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(EDeviceConnection::Unrecognized) == static_cast<uint8>(EDSDeviceConnection::Unrecognized), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(EGamepadHand::Left) == static_cast<uint8>(EDSGamepadHand::Left), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(EGamepadHand::Right) == static_cast<uint8>(EDSGamepadHand::Right), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(EGamepadHand::AnyHand) == static_cast<uint8>(EDSGamepadHand::AnyHand), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(EDualSenseAudioFeatureReport::On) == static_cast<uint8>(EDSAudioMode::On), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(EDualSenseAudioFeatureReport::Off) == static_cast<uint8>(EDSAudioMode::Off), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(ELedPlayerEnum::Off) == static_cast<uint8>(EDSPlayer::Off), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(ELedPlayerEnum::One) == static_cast<uint8>(EDSPlayer::One), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(ELedPlayerEnum::Two) == static_cast<uint8>(EDSPlayer::Two), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(ELedPlayerEnum::Three) == static_cast<uint8>(EDSPlayer::Three), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(ELedPlayerEnum::All) == static_cast<uint8>(EDSPlayer::All), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(ELedMicEnum::MicOn) == static_cast<uint8>(EDSMic::MicOn), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(ELedMicEnum::MicOff) == static_cast<uint8>(EDSMic::MicOff), "Enum mismatch: DualShock4 value differs between Core and API!");
static_assert(static_cast<uint8>(ELedMicEnum::Pulse) == static_cast<uint8>(EDSMic::Pulse), "Enum mismatch: DualShock4 value differs between Core and API!");

/**
 * @namespace SonyGamepadProxyHelpers
 * @brief Utility functions to reduce boilerplate code in Blueprint proxy classes.
 *
 * Thread Safety: All functions must be called from the Game Thread
 */
namespace SonyGamepadProxyHelpers
{
	/**
	 * @brief Finds the ISonyGamepad instance for a given controller ID.
	 *
	 * @param ControllerId The platform user ID (0-3 typically)
	 * @param bLogOnFailure Whether to log when gamepad is not found
	 * @return ISonyGamepad pointer or nullptr if not found
	 */
	inline ISonyGamepad* GetGamepad(int32 ControllerId, bool bLogOnFailure = true)
	{
		check(IsInGameThread());
		
		TArray<FInputDeviceId> Devices;
		IPlatformInputDeviceMapper::Get().GetAllInputDevicesForUser(
		    FPlatformUserId::CreateFromInternalId(ControllerId),
		    Devices);

		FDeviceRegistry* Registry = FDeviceRegistry::Get();
		for (const FInputDeviceId& DeviceId : Devices)
		{
			if (ISonyGamepad* Gamepad = Registry->GetLibraryInstance(DeviceId))
			{
				return Gamepad;
			}
		}

		if (bLogOnFailure)
		{
			UE_LOG(LogDualSense, Warning,
			       TEXT("Controller %d not found or not a Sony gamepad"),
			       ControllerId);
		}

		return nullptr;
	}

	/**
	 * @brief Gets the trigger interface for a controller.
	 *
	 * @param ControllerId The controller ID
	 * @param bLogOnFailure Whether to log failures
	 * @return IGamepadTrigger pointer or nullptr
	 */
	inline IGamepadTrigger* GetTriggerInterface(int32 ControllerId, bool bLogOnFailure = true)
	{
		ISonyGamepad* Gamepad = GetGamepad(ControllerId, bLogOnFailure);
		if (!Gamepad)
		{
			return nullptr;
		}

		IGamepadTrigger* TriggerInterface = Gamepad->GetIGamepadTrigger();

		if (!TriggerInterface && bLogOnFailure)
		{
			UE_LOG(LogDualSense, Warning,
			       TEXT("Controller %d does not support trigger interface"),
			       ControllerId);
		}

		return TriggerInterface;
	}

	/**
	 * @brief Gets the audio haptics interface for a controller.
	 *
	 * @param ControllerId The controller ID
	 * @param bLogOnFailure Whether to log failures
	 * @return IGamepadAudioHaptics pointer or nullptr
	 */
	inline IGamepadAudioHaptics* GetAudioHapticsInterface(int32 ControllerId, bool bLogOnFailure = true)
	{
		ISonyGamepad* Gamepad = GetGamepad(ControllerId, bLogOnFailure);
		if (!Gamepad)
		{
			return nullptr;
		}

		IGamepadAudioHaptics* AudioInterface = Gamepad->GetIGamepadHaptics();

		if (!AudioInterface && bLogOnFailure)
		{
			UE_LOG(LogDualSense, Warning,
			       TEXT("Controller %d does not support audio haptics interface"),
			       ControllerId);
		}

		return AudioInterface;
	}

	/**
	 * @brief Converts ControllerId to FInputDeviceId (for low-level operations).
	 *
	 * @param ControllerId The controller ID
	 * @return Valid FInputDeviceId or invalid ID if not found
	 */
	inline FInputDeviceId GetDeviceId(int32 ControllerId)
	{
		check(IsInGameThread());
		
		TArray<FInputDeviceId> Devices;
		IPlatformInputDeviceMapper::Get().GetAllInputDevicesForUser(
		    FPlatformUserId::CreateFromInternalId(ControllerId),
		    Devices);

		FDeviceRegistry* Registry = FDeviceRegistry::Get();
		for (const FInputDeviceId& DeviceId : Devices)
		{
			if (Registry->GetLibraryInstance(DeviceId))
			{
				return DeviceId;
			}
		}

		return FInputDeviceId::CreateFromInternalId(INDEX_NONE);
	}

	inline std::vector<std::uint8_t> ConvertVector(const TArray<FString>& UnrealArray)
	{
		std::vector<uint8_t> CoreBytes;
		CoreBytes.reserve(UnrealArray.Num());
		for (const FString& HexStr : UnrealArray)
		{
			CoreBytes.push_back(static_cast<uint8_t>(FParse::HexNumber(*HexStr)));
		}
		return CoreBytes;
	}

} // namespace SonyGamepadProxyHelpers
