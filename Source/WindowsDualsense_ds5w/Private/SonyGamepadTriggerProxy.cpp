// Fill out your copyright notice in the Description page of Project Settings.


#include "SonyGamepadTriggerProxy.h"
#include "Core/Enums/EDeviceCommons.h"
#include "Core/DeviceRegistry.h"


void USonyGamepadTriggerProxy::GameCube(int32 ControllerId, EGamepadHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}
	Gamepad->SetGameCube(static_cast<EControllerHand>(Hand));
}

void USonyGamepadTriggerProxy::Resistance(int32 ControllerId, ETriggerPositionMask StartPosition,
                                          ETriggerForceMask Strength, EGamepadHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetResistance(static_cast<uint8>(StartPosition), static_cast<uint8>(Strength), static_cast<EControllerHand>(Hand));
}


void USonyGamepadTriggerProxy::Bow(int32 ControllerId, ETriggerPosition StartZone, EDualSenseSnapBack SnapBack,
                                   EGamepadHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetBow22(static_cast<uint8>(StartZone), static_cast<uint8>(SnapBack), static_cast<EControllerHand>(Hand));
}

void USonyGamepadTriggerProxy::Weapon(int32 ControllerId, ETriggerPosition StartZone,
                                      EDualSenseTriggerAmplitude Amplitude, ETriggerEffectBehavior Behavior,
                                      EDualSenseWeaponTrigger Trigger, EGamepadHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetWeapon25(static_cast<uint8>(StartZone), static_cast<uint8>(Amplitude), static_cast<uint8>(Behavior),
	                     static_cast<uint8>(Trigger), static_cast<EControllerHand>(Hand));
}

void USonyGamepadTriggerProxy::AutomaticGun(int32 ControllerId, ETriggerPosition StartZone,
                                            ETriggerEffectBehavior Behavior, EAutoGunStrength Recoil, FSliderPropertyStruct Frequency, EGamepadHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetMachineGun26(static_cast<uint8>(StartZone), static_cast<uint8>(Behavior), static_cast<uint8>(Recoil), Frequency.Frequency, static_cast<EControllerHand>(Hand));
}

void USonyGamepadTriggerProxy::Machine(int32 ControllerId, ETriggerPosition StartZone, ETriggerEffectBehavior Behavior,
                                       ETriggerForceIntensity ForceIntensity, EDualSenseTriggerAmplitude Amplitude,
                                       FSliderPropertiesStruct Frequency_Period,
                                       EGamepadHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetMachine27(static_cast<uint8>(StartZone), static_cast<uint8>(Behavior), static_cast<uint8>(ForceIntensity), static_cast<uint8>(Amplitude),
	                      Frequency_Period.Period, Frequency_Period.Frequency, static_cast<EControllerHand>(Hand));
}

void USonyGamepadTriggerProxy::CustomTrigger(int32 ControllerId, EGamepadHand Hand, const TArray<FString>& HexBytes)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	if (HexBytes.Num() > 10)
	{
		return;
	}
	Gamepad->SetCustomTrigger(static_cast<EControllerHand>(Hand), HexBytes);
}

void USonyGamepadTriggerProxy::StopTrigger(int32 ControllerId, EGamepadHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->StopTrigger(static_cast<EControllerHand>(Hand));
}

IGamepadTrigger* USonyGamepadTriggerProxy::GetGamepadInterface(int32 ControllerId)
{
	// We should never call into IPlatformInputDeviceMapper from non-game thread because it is not thread-safe
	check(IsInGameThread());

	TArray<FInputDeviceId> Devices;
	IPlatformInputDeviceMapper::Get().GetAllInputDevicesForUser(FPlatformUserId::CreateFromInternalId(ControllerId), Devices);
	for (const FInputDeviceId& DeviceId : Devices)
	{
		if (ISonyGamepad* IGamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId))
		{
			return IGamepad->GetIGamepadTrigger();
		}
	}
	return nullptr;
}
