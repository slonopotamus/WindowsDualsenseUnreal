// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "../../Public/Subsystems/AudioHapticsListener.h"

#include "Core/DeviceRegistry.h"
#include "Core/Interfaces/SonyGamepadTriggerInterface.h"

FAudioHapticsListener::FAudioHapticsListener(const FInputDeviceId InDeviceId): DeviceId(InDeviceId),
                                                                               LastUpdateTime(0.0),
                                                                               AudioVibrationSequence(0)
{
    // ProcessedAudio.SetNum(142);
    // FMemory::Memset(ProcessedAudio.GetData(), 0, ProcessedAudio.Num());
}

FAudioHapticsListener::~FAudioHapticsListener()
{
    // FMemory::Memset(ProcessedAudio.GetData(), 0, ProcessedAudio.Num());
}

void FAudioHapticsListener::OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock)
{
    const double CurrentTime = AudioClock;
    if (CurrentTime - LastUpdateTime < 1.0 / 60.0)
    {
        return;
    }

    FDualSenseHapictBuffer* ProcessedAudio = new FDualSenseHapictBuffer();
    
    const float Threshold = 0.05f;
    const float ScaleFactor = 15.0f;
    const int32 AudioDataSize = 64;
    for (int32 i = 0; i < AudioDataSize; ++i)
    {
        float MaxAmplitude = 0.0f;
        const int32 StartSample = i * (NumSamples / AudioDataSize);
        const int32 EndSample = FMath::Min(StartSample + (NumSamples / AudioDataSize), NumSamples);

        for (int32 j = StartSample; j < EndSample; j++)
        {
            const float Sample = AudioData[j * NumChannels];
            MaxAmplitude = FMath::Max(MaxAmplitude, FMath::Abs(Sample));
        }

        if (MaxAmplitude < Threshold)
        {
            MaxAmplitude = 0.0f;
        }
        else
        {
            MaxAmplitude = FMath::Min(MaxAmplitude * ScaleFactor, 0.5f);
        }
        
        ProcessedAudio->report.pkt12.data[0 + i] = static_cast<uint8>(MaxAmplitude * 127.0f);
    }

    ProcessedAudio->report.pkt11.data[6] = AudioVibrationSequence++;
    const uint32 Crc = CalculateCRC32(ProcessedAudio->raw, 138);

    // CRC little-endian
    ProcessedAudio->raw[138] = Crc & 0xFF;
    ProcessedAudio->raw[139] = (Crc >> 8) & 0xFF;
    ProcessedAudio->raw[140] = (Crc >> 16) & 0xFF;
    ProcessedAudio->raw[141] = (Crc >> 24) & 0xFF;

    if (ISonyGamepadTriggerInterface* DualSense = Cast<ISonyGamepadTriggerInterface>(
        FDeviceRegistry::Get()->GetLibraryInstance(DeviceId)))
    {
        DualSense->AudioHapticUpdate(ProcessedAudio);
        LastUpdateTime = CurrentTime;
    }
}

