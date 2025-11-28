// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "DualSenseProxy.h"
#include "Core/DeviceRegistry.h"
#include "Core/DualSense/DualSenseLibrary.h"
#include "Core/Interfaces/ISonyGamepad.h"
#include "Helpers/ValidateHelpers.h"

void UDualSenseProxy::DeviceSettings(int32 ControllerId, FDualSenseFeatureReport Settings)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}
	if (Gamepad && Gamepad->GetDeviceType() == EDeviceType::DualSense)
	{
		FDualSenseLibrary* DualSenseInstance = static_cast<FDualSenseLibrary*>(Gamepad);
		DualSenseInstance->Settings(Settings);
	}
}

void UDualSenseProxy::RegisterSubmixForDevice(int32 ControllerId, USoundSubmix* Submix)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	FHapticsRegistry::Get()->CreateListenerForDevice(DeviceId, Submix);
}

void UDualSenseProxy::UnregisterSubmixForDevice(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}
	FHapticsRegistry::Get()->RemoveListenerForDevice(DeviceId);
}

void UDualSenseProxy::LedPlayerEffects(int32 ControllerId, ELedPlayerEnum Value, ELedBrightnessEnum Brightness)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetPlayerLed(Value, Brightness);
}

void UDualSenseProxy::SetFeedback(int32 ControllerId, int32 BeginStrength,
                                  int32 MiddleStrength, int32 EndStrength, EControllerHand Hand)
{
}

void UDualSenseProxy::Resistance(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Strength, EControllerHand Hand)
{
}

void UDualSenseProxy::AutomaticGun(int32 ControllerId, int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, EControllerHand Hand, bool KeepEffect, float Frequency)
{
}

void UDualSenseProxy::GameCube(int32 ControllerId, EControllerHand Hand)
{
}

void UDualSenseProxy::CustomTrigger(int32 ControllerId, EControllerHand Hand, const TArray<FString>& HexBytes)
{
}

void UDualSenseProxy::ContinuousResistance(int32 ControllerId, int32 StartPosition, int32 Strength, EControllerHand Hand)
{
	if (!FValidateHelpers::ValidateMaxPosition(StartPosition))
	{
		StartPosition = 0;
	}
	if (!FValidateHelpers::ValidateMaxPosition(Strength))
	{
		Strength = 8;
	}

	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}

	if (IGamepadTrigger* DS = Gamepad->GetIGamepadTrigger())
	{
		DS->SetResistance(StartPosition, Strength, Hand);
	}
}

void UDualSenseProxy::Galloping(
	int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 FirstFoot,
	int32 SecondFoot, float Frequency, EControllerHand Hand)
{
}

void UDualSenseProxy::Machine(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 FirstFoot,
                              int32 LasFoot, float Frequency, float Period, EControllerHand Hand)
{
}

void UDualSenseProxy::Weapon(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 Strength,
                             EControllerHand Hand)
{
}

void UDualSenseProxy::Bow(int32 ControllerId, int32 StartPosition, int32 EndPosition, int32 BeginStrength, int32 EndStrength,
                          EControllerHand Hand)
{
}

void UDualSenseProxy::MachineAdvanced(int32 ControllerId, ETriggerPosition StartZone,
                                      ETriggerEffectBehavior Behavior,
                                      ETriggerForceIntensity ForceIntensity,
                                      EDualSenseTriggerAmplitude Amplitude,
                                      int32 Period, int32 Frequency, EControllerHand Hand)
{
}

void UDualSenseProxy::NoResistance(int32 ControllerId, EControllerHand Hand)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}

	if (IGamepadTrigger* DS = Gamepad->GetIGamepadTrigger())
	{
		DS->StopTrigger(Hand);
	}
}

void UDualSenseProxy::StopTriggerEffect(const int32 ControllerId, EControllerHand HandStop)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}

	if (IGamepadTrigger* DS = Gamepad->GetIGamepadTrigger())
	{
		DS->StopTrigger(HandStop);
	}
}

void UDualSenseProxy::StopAllTriggersEffects(const int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	IGamepadTrigger* Gamepad = reinterpret_cast<IGamepadTrigger*>(FDeviceRegistry::Get()->GetLibraryInstance(DeviceId));
	if (!Gamepad)
	{
		return;
	}

	Gamepad->StopTrigger(EControllerHand::AnyHand);
}

void UDualSenseProxy::ResetEffects(const int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}

	ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->ResetLights();
}
