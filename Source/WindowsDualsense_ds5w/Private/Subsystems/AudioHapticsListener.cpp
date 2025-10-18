// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "../../Public/Subsystems/AudioHapticsListener.h"

#include "Core/DeviceRegistry.h"
#include "Core/Interfaces/SonyGamepadTriggerInterface.h"

FAudioHapticsListener::FAudioHapticsListener(const FInputDeviceId InDeviceId): DeviceId(InDeviceId), LastUpdateTime(0.0)
{
}

FAudioHapticsListener::~FAudioHapticsListener()
{
}

void FAudioHapticsListener::OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock)
{
	UE_LOG(LogTemp, Log, TEXT("AudioHapticsListener::OnNewSubmixBuffer %s"), *OwningSubmix->GetName());
	UE_LOG(LogTemp, Log, TEXT("AudioHapticsListener::Params %f, %d, %d, %d, %f"), AudioData[0], NumSamples, NumChannels, SampleRate, AudioClock);

	// Para ver o endereço do buffer
	UE_LOG(LogTemp, Log, TEXT("AudioData buffer address: %p"), AudioData);

    const double CurrentTime = AudioClock;
    const double MinUpdateInterval = 1.0 / 60.0;

    if (CurrentTime - LastUpdateTime < MinUpdateInterval)
    {
        return;
    }

    TArray<uint8> ProcessedAudio;
    ProcessedAudio.SetNum(64);

    // Simplificar o processamento e aumentar a sensibilidade
    const float AmplitudeMultiplier = 127.0f; // Aumentar a sensibilidade
    const int32 SamplesPerOutput = (NumSamples * NumChannels) / 64;
    
    for (int32 i = 0; i < 64; ++i)
    {
        float MaxAmplitude = 0.0f;
        const int32 StartSample = i * SamplesPerOutput;
        const int32 EndSample = FMath::Min(StartSample + SamplesPerOutput, NumSamples * NumChannels);
        
        // Pegar o valor máximo para este segmento
        for (int32 j = StartSample; j < EndSample; j++)
        {
            MaxAmplitude = FMath::Max(MaxAmplitude, FMath::Abs(AudioData[j]));
        }
        
        // Aplicar multiplicador e converter para byte
        float ScaledAmplitude = FMath::Clamp(MaxAmplitude * AmplitudeMultiplier, 0.0f, 1.0f);
        ProcessedAudio[i] = static_cast<uint8>(ScaledAmplitude * 255.0f);
    }

    if (ISonyGamepadTriggerInterface* DualSense = Cast<ISonyGamepadTriggerInterface>(
        FDeviceRegistry::Get()->GetLibraryInstance(DeviceId)))
    {
        // const uint8 AudioPattern[64] = ;

        // TArray<uint8> NewDateProcess;
        // NewDateProcess.SetNum(64);
        // for (int i = 0; i < 64; i++)
        // {
        //     NewDateProcess.Add(AudioPattern[i]);
        // }
        FPlatformProcess::Sleep(0.016f);
        DualSense->AudioHapticUpdate(ProcessedAudio);
        LastUpdateTime = CurrentTime;
    }
}

