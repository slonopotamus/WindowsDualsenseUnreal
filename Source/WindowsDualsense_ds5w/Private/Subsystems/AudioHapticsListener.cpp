// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "../../Public/Subsystems/AudioHapticsListener.h"
#include "Core/DeviceRegistry.h"
#include "Core/Interfaces/SonyGamepadTriggerInterface.h"
#include "Core/Structs/FDualSenseFeatureReport.h"

FAudioHapticsListener::FAudioHapticsListener(FInputDeviceId InDeviceId) : DeviceId(InDeviceId)
{
	ResampledAudioBuffer.SetNumUninitialized(64);
}


void FAudioHapticsListener::OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples,
                                              int32 NumChannels, const int32 SampleRate, double AudioClock)
{
    if (!ResamplerImpl.IsValid())
    {
        float InputSampleRate = (float)SampleRate;
        float OutputSampleRate = 3000.0f;
        float Ratio = OutputSampleRate / InputSampleRate;
        const int32 ResamplerChannels = 1;

        ResamplerImpl = MakeUnique<Audio::FResampler>();
        ResamplerImpl->Init(
            Audio::EResamplingMethod::Linear, 
            Ratio, 
            ResamplerChannels 
        );

    }
    int32 NumInputFrames = NumSamples / NumChannels;
    MonoMixBuffer.SetNumUninitialized(NumInputFrames);

    if (NumChannels == 2)
    {
        for (int32 i = 0; i < NumInputFrames; ++i)
        {
            float LeftSample = AudioData[i * 2];
            float RightSample = AudioData[i * 2 + 1];
            MonoMixBuffer[i] = (LeftSample + RightSample) * 0.5f;
        }
    }
    else
    {
        for (int32 i = 0; i < NumInputFrames; ++i)
        {
            MonoMixBuffer[i] = AudioData[i * NumChannels];
        }
    }

    int32 ExpectedOutputFrames = FMath::CeilToInt((float)NumInputFrames * 2);
    ResampledAudioBuffer.SetNumUninitialized(ExpectedOutputFrames + 32);

    int32 OutputFramesWritten = 0;
    ResamplerImpl->ProcessAudio(
        MonoMixBuffer.GetData(),
        NumInputFrames,
        false,
        ResampledAudioBuffer.GetData(),
        ResampledAudioBuffer.Num(),
        OutputFramesWritten
    );

    if (OutputFramesWritten != 64)
    {
        UE_LOG(LogTemp, Warning, TEXT("OutputFramesWritten not 64 bytes! (%d)"), OutputFramesWritten);
        return;
    }
    
    float* ResampledData = ResampledAudioBuffer.GetData();

    TArray<int8> Packet1, Packet2;
    Packet1.SetNumUninitialized(64);
    Packet2.SetNumUninitialized(64);

    for (int32 i = 0; i < 32; ++i)
    {
        int8 SampleInt8 = (int8)FMath::Clamp(FMath::RoundToInt(ResampledData[i] * 127.0f), -128, 127);
        Packet1[i * 2]     = SampleInt8;
        Packet1[i * 2 + 1] = SampleInt8;
    }
    
    for (int32 i = 0; i < 32; ++i)
    {
        int8 SampleInt8 = (int8)FMath::Clamp(FMath::RoundToInt(ResampledData[i + 32] * 127.0f), -128, 127);
        Packet2[i * 2]     = SampleInt8;
        Packet2[i * 2 + 1] = SampleInt8;
    }

    if (ISonyGamepadTriggerInterface* DualSense = Cast<ISonyGamepadTriggerInterface>(
            FDeviceRegistry::Get()->GetLibraryInstance(DeviceId)))
    {
        DualSense->AudioHapticUpdate(Packet1);
        DualSense->AudioHapticUpdate(Packet2);
    }
}
