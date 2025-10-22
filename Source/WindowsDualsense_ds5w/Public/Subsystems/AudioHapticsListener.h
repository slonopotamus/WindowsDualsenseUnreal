// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "ISubmixBufferListener.h"
#include "Sound/SoundSubmix.h"
#include "Containers/Queue.h"
#include "Core/Structs/FDeviceContext.h"

/**
 Class responsible for handling audio submix buffers and preparing audio data for haptic feedback systems.
 
 FAudioHapticsListener integrates with the audio rendering pipeline using the ISubmixBufferListener interface,
 allowing real-time access to submix audio buffers. The class supports processing, conversion, and resampling
 of audio data for use in haptic feedback hardware or systems. It includes mechanisms to manage resampling state
 and an internal queue for storing processed audio data.
 */
class FAudioHapticsListener final : public ISubmixBufferListener
{
public:
	explicit FAudioHapticsListener(FInputDeviceId InDeviceId);
	virtual ~FAudioHapticsListener() override;
	
	/**
	Called when a new buffer has been rendered for a given submix
	@param OwningSubmix	The submix object which has rendered a new buffer
	@param AudioData		Ptr to the audio buffer
	@param NumSamples		The number of audio samples in the audio buffer
	@param NumChannels		The number of channels of audio in the buffer (e.g. 2 for stereo, 6 for 5.1, etc)
	@param SampleRate		The sample rate of the audio buffer
	@param AudioClock		Double audio clock value, from start of audio rendering.
	*/
	virtual void OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock) override;
private:
	FInputDeviceId DeviceId;
	double LastUpdateTime;
};