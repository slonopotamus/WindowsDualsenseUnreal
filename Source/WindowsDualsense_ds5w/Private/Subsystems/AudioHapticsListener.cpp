// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "../../Public/Subsystems/AudioHapticsListener.h"

#include "Core/DeviceRegistry.h"
#include "Core/Interfaces/SonyGamepadTriggerInterface.h"
#include "Core/Structs/FDualSenseFeatureReport.h"

FAudioHapticsListener::FAudioHapticsListener(const FInputDeviceId InDeviceId): DeviceId(InDeviceId),
                                                                               LastUpdateTime(0.0)
{
}

FAudioHapticsListener::~FAudioHapticsListener()
{
}

void FAudioHapticsListener::OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock)
{
    if (!OwningSubmix) 
    {
        UE_LOG(LogTemp, Error, TEXT("Submix is NULL! Cannot create listener"));
        return;
    }

    const double CurrentTime = FPlatformTime::Seconds();
    // 3kHz (0.00033s ou 0.33ms entre pacotes)
    // ~1.5kHz (0.00066s)
    if (const double TimeSinceLastUpdate = CurrentTime - LastUpdateTime; TimeSinceLastUpdate < 0.00066)
    {
        return; // Skip este frame
    }
    LastUpdateTime = CurrentTime;
    
    if (ISonyGamepadTriggerInterface* DualSense = Cast<ISonyGamepadTriggerInterface>(
        FDeviceRegistry::Get()->GetLibraryInstance(DeviceId)))
    {
        DualSense->AudioHapticUpdate(AudioData, NumSamples, NumChannels, SampleRate, AudioClock);
    }
}

