// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025
// Acknowledgement: USB Audio Haptics logic based on research and code shared by yncat (https://github.com/yncat)
// in Issue #105: https://github.com/rafaelvaloto/Unreal-Dualsense/issues/105

#include "Subsystems/AudioHapticsListener.h"
#include "API/SonyGamepadProxyHelpers.h"
#include "GCore/Interfaces/Segregations/IGamepadAudioHaptics.h"

constexpr float kLowPassAlpha = 0.98f;
constexpr float one_minus_alpha = 1.0f - kLowPassAlpha;

FAudioHapticsListener::FAudioHapticsListener(int32 InDeviceId, USoundSubmix* InSubmix, bool bIsWireless)
    : Submix(InSubmix)
    , DeviceId(InDeviceId)
    , bIsWireless(bIsWireless)
{

	if (bIsWireless)
	{
		ResampledAudioBuffer.SetNumUninitialized(64);
	}
	else
	{
		ResampledAudioBuffer.SetNumUninitialized(4096);
	}
}

void FAudioHapticsListener::OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples,
                                              int32 NumChannels, const int32 SampleRate, double AudioClock)
{
	// ----------------------------------------------------------------------------------------------------------------
	// Acknowledgement:
	// The following USB Audio Haptics processing logic (LowPass filter & buffer handling) is based on
	// research and implementation originally shared by yncat (https://github.com/yncat).
	// Reference Issue #105: https://github.com/rafaelvaloto/Unreal-Dualsense/issues/105
	// ----------------------------------------------------------------------------------------------------------------
	if (!bIsWireless) // USB
	{
		if (NumSamples <= 0)
		{
			return;
		}

		std::vector<std::int16_t> StereoPair;
		StereoPair.resize(2);
		for (int32 i = 0; i < NumSamples; i += NumChannels)
		{
			float InLeft = AudioData[i];
			float InRight = (NumChannels > 1) ? AudioData[i + 1] : InLeft;

			LowPassState_Left = one_minus_alpha * InLeft + kLowPassAlpha * LowPassState_Left;
			LowPassState_Right = one_minus_alpha * InRight + kLowPassAlpha * LowPassState_Right;

			float OutLeft = FMath::Clamp(InLeft - LowPassState_Left, -1.0f, 1.0f);
			float OutRight = FMath::Clamp(InRight - LowPassState_Right, -1.0f, 1.0f);

			StereoPair[0] = static_cast<std::int16_t>(OutLeft * 32767.0f);
			StereoPair[1] = static_cast<std::int16_t>(OutRight * 32767.0f);
			AudioPacketQueueUSB.Enqueue(StereoPair);
		}
		return;
	}

	if (!ResamplerImpl.IsValid())
	{
		const float Ratio = 3000.0f / SampleRate;
		ResamplerImpl = MakeUnique<Audio::FResampler>();
		ResamplerImpl->Init(
		    Audio::EResamplingMethod::BestSinc,
		    Ratio,
		    NumChannels);
	}

	const int32 NumInputFrames = NumSamples / NumChannels; // (2048 samples / 2 channels = 1024 frames)

	// (1024 frames * (3000/48000)) = 64 frames.
	const int32 ExpectedOutputFrames = FMath::CeilToInt(static_cast<float>(NumInputFrames) * (3000.0f / SampleRate));
	ResampledAudioBuffer.SetNumUninitialized((ExpectedOutputFrames + 32) * NumChannels);

	int32 OutputFramesWritten = 0;
	ResamplerImpl->ProcessAudio(
	    AudioData,
	    NumInputFrames,
	    false,
	    ResampledAudioBuffer.GetData(),
	    ResampledAudioBuffer.Num() / NumChannels,
	    OutputFramesWritten);

	if (OutputFramesWritten != 64)
	{
		return;
	}

	float* Data = ResampledAudioBuffer.GetData();
	const int32 NumFrames = OutputFramesWritten; // 64 frames

	for (int32 i = 0; i < NumFrames; ++i)
	{
		const int32 DataIndex = i * NumChannels; // (i * 2)

		const float InLeft = Data[DataIndex];
		const float InRight = Data[DataIndex + 1];

		// y_lp[n] = (1 - alpha) * x[n] + alpha * y_lp[n-1]
		LowPassState_Left = one_minus_alpha * InLeft + kLowPassAlpha * LowPassState_Left;
		LowPassState_Right = one_minus_alpha * InRight + kLowPassAlpha * LowPassState_Right;

		// y_hp[n] = x[n] - y_lp[n]
		const float OutLeft = InLeft - LowPassState_Left;
		const float OutRight = InRight - LowPassState_Right;

		Data[DataIndex] = OutLeft;
		Data[DataIndex + 1] = OutRight;
	}

	const float* ResampledData = ResampledAudioBuffer.GetData();
	std::vector<std::int8_t> Packet1, Packet2;
	Packet1.reserve(64);
	Packet2.reserve(64);

	for (int32 i = 0; i < 32; ++i)
	{
		const int32 DataIndex = i * 2;

		const float LeftSample = ResampledData[DataIndex];      // L
		const float RightSample = ResampledData[DataIndex + 1]; // R

		const std::int8_t LeftSampleInt8 = static_cast<std::int8_t>(FMath::Clamp(FMath::RoundToInt(LeftSample * 127.0f), -128, 127));
		const std::int8_t RightSampleInt8 = static_cast<std::int8_t>(FMath::Clamp(FMath::RoundToInt(RightSample * 127.0f), -128, 127));
		Packet1[DataIndex] = LeftSampleInt8;      // L
		Packet1[DataIndex + 1] = RightSampleInt8; // R
	}

	// (Frames 32-63)
	for (int32 i = 0; i < 32; ++i)
	{
		// Freame 32 (i + 32) * 2
		const int32 DataIndex = (i + 32) * 2; // ResampledData (64, 66, 68...)

		const float LeftSample = ResampledData[DataIndex];
		const float RightSample = ResampledData[DataIndex + 1];

		const std::int8_t LeftSampleInt8 = static_cast<std::int8_t>(FMath::Clamp(FMath::RoundToInt(LeftSample * 127.0f), -128, 127));
		const std::int8_t RightSampleInt8 = static_cast<std::int8_t>(FMath::Clamp(FMath::RoundToInt(RightSample * 127.0f), -128, 127));

		// Index *Packet 2* (Packet2)
		const int32 PacketIndex = i * 2;            // (0, 2, 4...)
		Packet2[PacketIndex] = LeftSampleInt8;      // L
		Packet2[PacketIndex + 1] = RightSampleInt8; // R
	}

	AudioPacketQueue.Enqueue(Packet1);
	AudioPacketQueue.Enqueue(Packet2);
}

void FAudioHapticsListener::ConsumeHapticsQueue(IGamepadAudioHaptics* AudioHaptics)
{

	if (AudioHaptics && bIsWireless)
	{
		std::vector<std::int8_t> PacketToProcess;
		std::vector<std::int8_t> Samples;
		PacketToProcess.reserve(64);
		while (AudioPacketQueue.Dequeue(PacketToProcess))
		{
			if (PacketToProcess.size() == 0)
			{
				continue;
			}

			Samples.insert(Samples.end(), PacketToProcess.begin(), PacketToProcess.end());
			AudioHaptics->AudioHapticUpdate(Samples);
		}
	}
	else
	{
		std::vector<std::int16_t> QSamplePair;
		QSamplePair.reserve(2);

		std::vector<std::int16_t> Samples;
		Samples.reserve(2048 * 2);
		while (AudioPacketQueueUSB.Dequeue(QSamplePair))
		{
			if (QSamplePair.size() < 2)
			{
				continue;
			}

			std::int16_t SampleLeft = QSamplePair[0];
			std::int16_t SampleRight = QSamplePair[1];

			Samples.push_back(SampleLeft);
			Samples.push_back(SampleRight);
		}

		if (!Samples.empty() && AudioHaptics)
		{
			AudioHaptics->AudioHapticUpdate(Samples);
		}
	}

	AudioPacketQueue.Empty();
	AudioPacketQueueUSB.Empty();
}
