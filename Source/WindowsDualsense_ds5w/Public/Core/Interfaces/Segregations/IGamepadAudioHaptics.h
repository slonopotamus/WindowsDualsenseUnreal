// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGamepadAudioHaptics.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGamepadAudioHaptics : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WINDOWSDUALSENSE_DS5W_API IGamepadAudioHaptics
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Updates the haptic feedback on a gamepad's triggers using audio waveform data.
	 *
	 * @param AudioData An array of integer values representing the audio waveform data
	 *                  used to drive the haptic feedback effects on the triggers.
	 */
	virtual void AudioHapticUpdate(TArray<int8> AudioData) = 0;
};
