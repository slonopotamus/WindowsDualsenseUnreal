// Fill out your copyright notice in the Description page of Project Settings.


#include "SonyGamepadTriggerProxy.h"
#include "Core/Enums/EDeviceCommons.h"
#include "Core/DeviceRegistry.h"


void USonyGamepadTriggerProxy::GameCube(int32 ControllerId, EControllerHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}
	Gamepad->SetGameCube(Hand);
}

void USonyGamepadTriggerProxy::Bow(int32 ControllerId, ETriggerPosition StartZone, EDualSenseSnapBack SnapBack,
                                   EControllerHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetBow22(static_cast<uint8>(StartZone), static_cast<uint8>(SnapBack), Hand);
}

void USonyGamepadTriggerProxy::Weapon(int32 ControllerId, ETriggerPosition StartZone,
                                      EDualSenseTriggerAmplitude Amplitude, ETriggerEffectBehavior Behavior,
                                      EDualSenseWeaponTrigger Trigger, EControllerHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetWeapon25(static_cast<uint8>(StartZone), static_cast<uint8>(Amplitude), static_cast<uint8>(Behavior),
	                     static_cast<uint8>(Trigger), Hand);
}

void USonyGamepadTriggerProxy::Machine(int32 ControllerId, ETriggerPosition StartZone, ETriggerEffectBehavior Behavior,
                                       ETriggerForceIntensity ForceIntensity, EDualSenseTriggerAmplitude Amplitude,
                                       int32 Period, int32 Frequency,
                                       EControllerHand Hand)
{
	IGamepadTrigger* Gamepad = GetGamepadInterface(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetMachine27(static_cast<uint8>(StartZone), static_cast<uint8>(Behavior), static_cast<uint8>(ForceIntensity), static_cast<uint8>(Amplitude),
	                      FMath::Clamp(Period, 0, 20), FMath::Clamp(Frequency, 0, 40), Hand);
}

IGamepadTrigger* USonyGamepadTriggerProxy::GetGamepadInterface(int32 ControllerId)
{
	// We should never call into IPlatformInputDeviceMapper from non-game thread because it is not thread-safe
	check(IsInGameThread());

	TArray<FInputDeviceId> Devices;

	IPlatformInputDeviceMapper::Get().GetAllInputDevicesForUser(FPlatformUserId::CreateFromInternalId(ControllerId), Devices);

	for (const FInputDeviceId& DeviceId : Devices)
	{
		if (IGamepadTrigger* IGamepad = Cast<IGamepadTrigger>(FDeviceRegistry::Get()->GetLibraryInstance(DeviceId)))
		{
			return IGamepad;
		}
	}

	return nullptr;
}
