// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "../../Public/Core/HapticsRegistry.h"
#include "AudioDevice.h"

TSharedPtr<FHapticsRegistry> FHapticsRegistry::Instance;

FHapticsRegistry::~FHapticsRegistry()
{
	for (auto& Pair : ControllerListeners)
	{
		RemoveListenerForDevice(Pair.Key);
	}
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
	UE_LOG(LogTemp, Log, TEXT("STARTing Registering listener for device"));
	if (!Submix) return;

	UE_LOG(LogTemp, Log, TEXT("Enabled Registering listener for device"));
	RemoveListenerForDevice(DeviceId);
	const TSharedPtr<FAudioHapticsListener> Listener = MakeShared<FAudioHapticsListener>(DeviceId);

	UE_LOG(LogTemp, Log, TEXT("Enabled Registering listener for device"));
	if (FAudioDevice* AudioDevice = GEngine->GetMainAudioDeviceRaw())
	{
		AudioDevice->RegisterSubmixBufferListener(Listener.Get(), Submix);
		ControllerListeners.Add(DeviceId, Listener);
		UE_LOG(LogTemp, Log, TEXT("Registering listener for device %d num %d"), DeviceId.GetId(), ControllerListeners.Num());
	}
}

void FHapticsRegistry::RemoveListenerForDevice(const FInputDeviceId& DeviceId)
{
	if (TSharedPtr<FAudioHapticsListener>* Listener = ControllerListeners.Find(DeviceId))
	{
		ControllerListeners.Remove(DeviceId);
	}
}
