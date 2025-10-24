// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "../../Public/Core/HapticsRegistry.h"
#include "AudioDevice.h"

TSharedPtr<FHapticsRegistry> FHapticsRegistry::Instance;

FHapticsRegistry::~FHapticsRegistry()
{
	RemoveAllListeners();
}

bool FHapticsRegistry::HasListenerForDevice(const FInputDeviceId& DeviceId) const
{
	return ControllerListeners.Contains(DeviceId);
}

TSharedPtr<FHapticsRegistry> FHapticsRegistry::Get()
{
	if (!Instance)
	{
		check(IsInGameThread());
		Instance = MakeShared<FHapticsRegistry>();
	}
	return Instance;
}

void FHapticsRegistry::CreateListenerForDevice(const FInputDeviceId& DeviceId, USoundSubmix* Submix)
{
	if (!Submix) return;
	
	RemoveListenerForDevice(DeviceId);
	const TSharedPtr<FAudioHapticsListener> Listener = MakeShared<FAudioHapticsListener>(DeviceId);

	if (FAudioDeviceHandle AudioDevice = GEngine->GetActiveAudioDevice())
	{
		AudioDevice->RegisterSubmixBufferListener(Listener.Get(), Submix);
		ControllerListeners.Add(DeviceId, Listener);
		UE_LOG(LogTemp, Log, TEXT("Registering listener for device %d num %d"), DeviceId.GetId(), ControllerListeners.Num());
	}
}


void FHapticsRegistry::RemoveAllListeners()
{
	if (FAudioDeviceHandle AudioDevice = GEngine->GetActiveAudioDevice())
	{
		for (auto& Pair : ControllerListeners)
		{
			AudioDevice->UnregisterSubmixBufferListener(Pair.Value.Get());
		}
	}
	ControllerListeners.Empty();
}

void FHapticsRegistry::RemoveListenerForDevice(const FInputDeviceId& DeviceId)
{
	if (const TSharedPtr<FAudioHapticsListener>* ExistingListener = ControllerListeners.Find(DeviceId))
	{
		if (FAudioDeviceHandle AudioDevice = GEngine->GetActiveAudioDevice())
		{
			AudioDevice->UnregisterSubmixBufferListener(ExistingListener->Get());
			UE_LOG(LogTemp, Log, TEXT("Unregistered haptics listener for device %d"), DeviceId.GetId());
		}
		ControllerListeners.Remove(DeviceId);
	}
}
