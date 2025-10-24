// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "../../Public/Subsystems/AudioHapticsListener.h"

#include "Core/DeviceRegistry.h"
#include "Core/Interfaces/SonyGamepadTriggerInterface.h"
#include "Core/Structs/FDualSenseFeatureReport.h"

FAudioHapticsListener::FAudioHapticsListener(const FInputDeviceId InDeviceId) : DeviceId(InDeviceId)
{
}

FAudioHapticsListener::~FAudioHapticsListener()
{
}

void FAudioHapticsListener::OnSubmixEnded() const
{
	// if (ISonyGamepadTriggerInterface* DualSense = Cast<ISonyGamepadTriggerInterface>(
	// 	FDeviceRegistry::Get()->GetLibraryInstance(DeviceId)))
	// {
	// 	DualSense->StopAudioHapticConsumer();
	// }
}

void FAudioHapticsListener::OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples,
                                              int32 NumChannels, const int32 SampleRate, double AudioClock)
{
	if (!OwningSubmix)
	{
		if (ISonyGamepadTriggerInterface* DualSense = Cast<ISonyGamepadTriggerInterface>(
			FDeviceRegistry::Get()->GetLibraryInstance(DeviceId)))
		{
			DualSense->StopAudioHapticConsumer();
		}

		UE_LOG(LogTemp, Error, TEXT("Submix is NULL! Cannot create listener"));
		return;
	}

	if (ISonyGamepadTriggerInterface* DualSense = Cast<ISonyGamepadTriggerInterface>(
		FDeviceRegistry::Get()->GetLibraryInstance(DeviceId)))
	{
		DualSense->AudioHapticUpdate(AudioData, NumSamples, NumChannels, SampleRate, AudioClock);
	}
}
