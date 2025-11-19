// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGamepadTriggerLegacy.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGamepadTriggerLegacy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WINDOWSDUALSENSE_DS5W_API IGamepadTriggerLegacy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Sets the resistance levels for the gamepad triggers across different stages
	 * and applies them to a specified controller hand.
	 *
	 * @param BeginStrength The resistance strength applied at the beginning point of the trigger pull.
	 * @param MiddleStrength The resistance strength applied during the middle point of the trigger pull.
	 * @param EndStrength The resistance strength applied at the end point of the trigger pull.
	 * @param Hand The controller hand (e.g., left or right) to which the resistance settings will be applied.
	 */
	virtual void SetResistance(int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, const EControllerHand& Hand) = 0;
	/**
	 * Sets the automatic gun trigger effects on a controller by defining resistance levels
	 * across multiple stages and optionally retaining the effect across actions.
	 *
	 * @param BeginStrength The resistance strength at the beginning stage of the trigger pull.
	 * @param MiddleStrength The resistance strength at the middle stage of the trigger pull.
	 * @param EndStrength The resistance strength at the end stage of the trigger pull.
	 * @param Hand The controller hand (e.g., left or right) where the effect will be applied.
	 * @param KeepEffect Indicates whether the applied effect should persist after the action.
	 */
	virtual void SetAutomaticGun(int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, const EControllerHand& Hand, bool KeepEffect, float Frequency) = 0;
	/**
	 * Configures continuous resistance for a gamepad trigger at a specific position
	 * with a defined level of strength for a given controller hand.
	 *
	 * @param StartPosition The starting position on the trigger where the resistance begins to take effect.
	 * @param Strength The level of resistance to be applied, typically within a valid range for the device.
	 * @param Hand The controller hand (e.g., left or right) where the resistance effect will be applied.
	 */
	virtual void SetContinuousResistance(int32 StartPosition, int32 Strength, const EControllerHand& Hand) = 0;
	/**
	 * Configures the galloping effect on a gamepad by specifying the start and end positions,
	 * foot timings, frequency, and the hand to which the effect is applied.
	 *
	 * @param StartPosition The starting position of the trigger effect.
	 * @param EndPosition The ending position of the trigger effect.
	 * @param FirstFoot The timing or position representing the impact of the first foot.
	 * @param SecondFoot The timing or position representing the impact of the second foot.
	 * @param Frequency The frequency of the galloping effect to determine its speed.
	 * @param Hand The controller hand (e.g., left or right) where the effect will be applied.
	 */
	virtual void SetGalloping(int32 StartPosition, int32 EndPosition, int32 FirstFoot, int32 SecondFoot, float Frequency, const EControllerHand& Hand) = 0;
	/**
	 * Configures a custom machine effect on the gamepad triggers by defining start and end positions,
	 * amplitude levels, frequency, and duration specifics for a given controller hand.
	 *
	 * @param StartPosition The starting position of the trigger effect.
	 * @param EndPosition The ending position of the trigger effect.
	 * @param AmplitudeBegin The initial amplitude level for the trigger effect.
	 * @param AmplitudeEnd The final amplitude level for the trigger effect.
	 * @param Frequency The frequency of the effect to control its repetition rate.
	 * @param Period The duration of one cycle of the effect.
	 * @param Hand The controller hand (e.g., left or right) where the effect will be applied.
	 */
	virtual void SetMachine(int32 StartPosition, int32 EndPosition, int32 AmplitudeBegin, int32 AmplitudeEnd, float Frequency, float Period, const EControllerHand& Hand) = 0;
	/**
	 * Configures the weapon effect on a gamepad's adaptive triggers based on specified parameters.
	 *
	 * @param StartPosition The starting position for the trigger effect as an integer value.
	 * @param EndPosition The ending position for the trigger effect as an integer value.
	 * @param Strength The intensity of the trigger effect as an integer value.
	 * @param Hand A reference to the hand (left or right) where the effect should be applied, represented by EControllerHand.
	 */
	virtual void SetWeapon(int32 StartPosition, int32 EndPosition, int32 Strength, const EControllerHand& Hand) = 0;
	/**
	 * Configures the bow tension effects on a gamepad trigger for a specified hand.
	 *
	 * @param StartPosition The starting position of the bow tension effect on the trigger.
	 * @param EndPosition The ending position of the bow tension effect on the trigger.
	 * @param BegingStrength The intensity of the bow tension `Resistance` effect at the starting position.
	 * @param EndStrength The intensity of the bow tension `SnapForce` effect at the ending position.
	 * @param Hand The controller hand to which the bow effect will be applied.
	 */
	virtual void SetBow(int32 StartPosition, int32 EndPosition, int32 BegingStrength, int32 EndStrength, const EControllerHand& Hand) = 0;
};
