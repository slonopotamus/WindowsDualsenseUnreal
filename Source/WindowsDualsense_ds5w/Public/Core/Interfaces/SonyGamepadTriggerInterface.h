// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Segregations/IGamepadAudioHaptics.h"
#include "Segregations/IGamepadTrigger.h"
#include "Segregations/IGamepadTriggerLegacy.h"
#include "Templates/SharedPointer.h"
#include "UObject/Interface.h"
#include "SonyGamepadTriggerInterface.generated.h"

UINTERFACE()
class WINDOWSDUALSENSE_DS5W_API USonyGamepadTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class WINDOWSDUALSENSE_DS5W_API ISonyGamepadTriggerInterface
{
	GENERATED_BODY()
public:
	virtual void SetTriggerResistance(const FInputDeviceTriggerResistanceProperty& Resistance) = 0;
	/**
	 * Sets the haptic feedback for a specific hand on the gamepad.
	 *
	 * @param Hand The hand to which the haptic feedback will be applied.
	 *             Typically, 0 represents the left hand, and 1 represents the right hand.
	 * @param Values A pointer to an FHapticFeedbackValues structure containing the haptic feedback details.
	 */
	virtual void SetHapticFeedback(int32 Hand, const FHapticFeedbackValues* Values) = 0;
	
};
