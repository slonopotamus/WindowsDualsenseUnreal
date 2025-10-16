// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "../../Public/Subsystems/AudioHapticsListener.h"

FAudioHapticsListener::FAudioHapticsListener()
{
}

FAudioHapticsListener::~FAudioHapticsListener()
{
}

void FAudioHapticsListener::OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples,
                                              int32 NumChannels, const int32 SampleRate, double AudioClock)
{
	UE_LOG(LogTemp, Log, TEXT("OnNewSubmixBuffer"));
	// TArray<uint8> ConvertedChunk;
	// ConvertAndResampleAudio(AudioData, NumSamples, NumChannels, SampleRate, ConvertedChunk);
 //    
	// if (ConvertedChunk.Num() == 64)
	// {
	// 	ConvertedAudioQueue.Enqueue(ConvertedChunk);
	// }
}
void FAudioHapticsListener::ConvertAndResampleAudio(const float* InAudioData, int32 InNumSamples, int32 InNumChannels, 
												  int32 InSampleRate, TArray<uint8>& OutBuffer)
{
	constexpr int32 TargetSampleRate = 3000;
	constexpr int32 NumOutputSamples = 64;
	const float SampleRatio = static_cast<float>(InSampleRate) / TargetSampleRate;

	OutBuffer.Reset();
	OutBuffer.Reserve(NumOutputSamples);

	for (int32 i = 0; i < NumOutputSamples; ++i)
	{
		const float InputSampleIndexFloat = (i * SampleRatio) + ResampleCarryOver;

		if (const int32 InputSampleIndex = FMath::FloorToInt(InputSampleIndexFloat); (InputSampleIndex + 1) * InNumChannels <= InNumSamples)
		{
			float MonoSampleValue = 0.f;
			for (int32 c = 0; c < InNumChannels; ++c)
			{
				MonoSampleValue += InAudioData[(InputSampleIndex * InNumChannels) + c];
			}
			MonoSampleValue /= InNumChannels;
			const int8 ConvertedSample = static_cast<int8>(FMath::Clamp(MonoSampleValue * 127.f, -128.f, 127.f));
			OutBuffer.Add(static_cast<uint8>(ConvertedSample));
		}
		else
		{
			OutBuffer.Add(0);
		}
	}
    
	ResampleCarryOver = FMath::Frac(NumOutputSamples * SampleRatio + ResampleCarryOver);
}

