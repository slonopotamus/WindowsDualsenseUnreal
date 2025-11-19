// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SonyGamepadProxy.h"
#include "Core/Enums/EDeviceCommons.h"
#include "Core/Interfaces/Segregations/IGamepadTrigger.h"
#include "UObject/Object.h"
#include "SonyGamepadTriggerProxy.generated.h"

/**
 * 
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API USonyGamepadTriggerProxy : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Configures the "GameCube" trigger effect on a DualSense controller.
	 *
	 * This function applies a special trigger effect designed to simulate the feedback characteristics
	 * associated with a GameCube-style response. It can be applied selectively to the left or right
	 * hand trigger on the specified DualSense controller.
	 *
	 * @param ControllerId The ID of the controller on which the effect is to be applied.
	 * @param Hand Specifies the hand (left or right) to which the trigger effect should be targeted.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Trigger", meta = (DisplayName = "GameCube (0x02)"))
	static void GameCube(
		int32 ControllerId,
		EControllerHand Hand);

	/**
	 * @brief Configures the "Bow" trigger effect on a DualSense controller.
	 *
	 * This function applies a specific trigger feedback effect designed to emulate
	 * the response of a bow, targeting the specified start zone, snap-back behavior,
	 * and controller hand. It customizes the feedback on the trigger to align with a bow-like
	 * mechanic.
	 *
	 * @param ControllerId The unique identifier of the controller on which the effect is applied.
	 * @param StartZone Specifies the initial trigger position where the effect begins (e.g., start, middle).
	 * @param SnapBack Defines the snap-back behavior of the trigger after it is released.
	 * @param Hand Indicates the hand (left or right) where the trigger effect is applied.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Trigger", meta = (DisplayName = "Bow (0x22)"))
	static void Bow(
		int32 ControllerId,
		ETriggerPosition StartZone,
		EDualSenseSnapBack SnapBack,
		EControllerHand Hand);

	/**
	 * @brief Configures the "Weapon" trigger effect on a DualSense controller.
	 *
	 * This function applies a weapon-specific feedback effect that emulates a responsive trigger pull characteristic
	 * commonly associated with weapon dynamics in games. The effect can be customized based on the starting trigger position,
	 * vibration amplitude, behavior, and specific weapon effect type. It can be applied to either the left or right trigger.
	 *
	 * @param ControllerId The unique identifier of the controller on which the effect is applied.
	 * @param StartZone Specifies the initial position on the trigger where the effect begins.
	 * @param Amplitude Determines the intensity of the vibration feedback.
	 * @param Behavior Defines the nature of the effect, such as localized or sustained.
	 * @param Trigger Specifies the weapon effect type to emulate (e.g., pistol, shotgun).
	 * @param Hand Indicates the hand (left or right) where the trigger effect is applied.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Trigger", meta = (DisplayName = "Weapon (0x25)"))
	static void Weapon(int32 ControllerId, ETriggerPosition StartZone, EDualSenseTriggerAmplitude Amplitude,
	                   ETriggerEffectBehavior Behavior,
	                   EDualSenseWeaponTrigger Trigger, EControllerHand Hand);


	/**
	 * @brief Configures the "Machine" trigger effect on a DualSense controller.
	 *
	 * This function applies a detailed trigger effect resembling a machine-like feedback pattern
	 * to the specified DualSense controller. The effect's characteristics, such as the start zone,
	 * behavior, force intensity, amplitude, period, and frequency, can be customized. The effect can
	 * be directed to either the left or right hand trigger.
	 *
	 * @param ControllerId The ID of the controller to apply the effect to.
	 * @param StartZone Specifies the starting position of the effect on the trigger.
	 * @param Behavior Defines the behavior of the effect (localized or sustained).
	 * @param ForceIntensity Simulates the strength of the force feedback effect.
	 * @param Amplitude Determines the vibration amplitude for the effect.
	 * @param Period Defines the duration of each feedback cycle (0-20 range).
	 * @param Frequency Specifies the frequency of oscillations for the effect (0-40 range).
	 * @param Hand Identifies the hand (left or right) where the trigger effect should be applied.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Trigger", meta = (DisplayName = "Machine (0x27)"))
	static void Machine(
		int32 ControllerId,
		ETriggerPosition StartZone,
		ETriggerEffectBehavior Behavior,
		ETriggerForceIntensity ForceIntensity,
		EDualSenseTriggerAmplitude Amplitude,
		UPARAM(DisplayName = "Period min: 0 max: 20",
			meta = (ClampMin = "0", ClampMax = "20", UIMin = "0", UIMax = "20"))
		int32 Period,
		UPARAM(DisplayName = "Frequency min: 0 max: 40",
			meta = (ClampMin = "0", ClampMax = "40", UIMin = "0", UIMax = "40"))
		int32 Frequency,
		EControllerHand Hand);

	
private:
	static IGamepadTrigger* GetGamepadInterface(int32 ControllerId);
};
