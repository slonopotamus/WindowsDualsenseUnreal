// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "ISubmixBufferListener.h"
#include "Sound/SoundSubmix.h"
#include "Containers/Queue.h"

/**
 Class responsible for handling audio submix buffers and preparing audio data for haptic feedback systems.
 
 FAudioHapticsListener integrates with the audio rendering pipeline using the ISubmixBufferListener interface,
 allowing real-time access to submix audio buffers. The class supports processing, conversion, and resampling
 of audio data for use in haptic feedback hardware or systems. It includes mechanisms to manage resampling state
 and an internal queue for storing processed audio data.
 */
class FAudioHapticsListener : public ISubmixBufferListener
{
public:
	FAudioHapticsListener();
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
	/**
	 Queue that stores arrays of converted and processed audio data for further handling.
	 
	 The audio data stored in this queue is typically converted and resampled from incoming audio buffers.
	 This allows the processed audio to be accessible for additional features or systems that require the transformed data.
	 */
	TQueue<TArray<uint8>> ConvertedAudioQueue;

	/**
	 Converts and resamples audio data for use in haptic feedback systems. This function takes input float audio data,
	 downsamples it to a target sample rate, converts it to mono, and encodes it into 8-bit integer format for output.
	 
	 @param InAudioData Pointer to the input audio buffer containing floating-point audio samples.
	 @param InNumSamples The total number of samples contained in the input audio buffer.
	 @param InNumChannels The number of channels in the input audio data (e.g., 1 for mono, 2 for stereo).
	 @param InSampleRate The sample rate of the input audio data in Hz (e.g., 44100).
	 @param OutBuffer A reference to a buffer where the processed and resampled audio data is stored as 8-bit integers.
	 */
private:
	void ConvertAndResampleAudio(const float* InAudioData, int32 InNumSamples, int32 InNumChannels, 
												  int32 InSampleRate, TArray<uint8>& OutBuffer);
	/**
	 Holds the fractional carry-over value during audio data resampling.
	 
	 This variable is used to accumulate leftover fractional positions when resampling
	 audio data, ensuring that the resampling process maintains accuracy over multiple iterations.
	 It accounts for partially consumed sample positions between resampling steps.
	 */
	float ResampleCarryOver = 0.f;
};