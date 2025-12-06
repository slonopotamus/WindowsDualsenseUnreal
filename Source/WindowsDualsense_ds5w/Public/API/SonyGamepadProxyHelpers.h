// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "Core/Interfaces/ISonyGamepad.h"
#include "Core/Managers/DeviceRegistry.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "Helpers/DualSenseLog.h"

// Forward declarations
class IGamepadTrigger;
class IGamepadAudioHaptics;

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

		for (const FInputDeviceId& DeviceId : Devices)
		{
			if (ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId))
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

		for (const FInputDeviceId& DeviceId : Devices)
		{
			if (FDeviceRegistry::Get()->GetLibraryInstance(DeviceId))
			{
				return DeviceId;
			}
		}

		return FInputDeviceId::CreateFromInternalId(INDEX_NONE);
	}

} // namespace SonyGamepadProxyHelpers
