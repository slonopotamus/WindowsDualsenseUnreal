// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGamepadTrigger.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGamepadTrigger : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WINDOWSDUALSENSE_DS5W_API IGamepadTrigger
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Disables the trigger effects on a gamepad for the specified controller hand.
	 *
	 * @param Hand An enum indicating which controller hand's trigger effects should be stopped.
	 */
	virtual void StopTrigger(const EControllerHand& Hand) = 0;
	/**
	 * Configures the GameCube controller interface for a specified controller hand.
	 *
	 * @param Hand The controller hand (left or right) for which the GameCube interface
	 *             configuration is applied.
	 */
	virtual void SetGameCube(const EControllerHand& Hand) = 0;
	/**
	 * Activates custom trigger configurations on a gamepad for a specified hand.
	 *
	 * @param Hand A reference to the EControllerHand enum specifying the hand
	 *             (e.g., left or right) for which the trigger is being configured.
	 * @param HexBytes An array of strings representing hexadecimal values that define
	 *                 the trigger configuration parameters.
	 */
	virtual void SetCustomTrigger(const EControllerHand& Hand, const TArray<FString>& HexBytes) = 0;
	/**
	 * Configures a custom feedback mechanism with specified amplitude and zone parameters for a gamepad trigger.
	 *
	 * @param StartZones A value defining the starting zones of the trigger feedback effect.
	 * @param Amplitude The amplitude level for the first zone of the feedback effect.
	 * @param Hand The controller hand (e.g., left or right) for which the feedback configuration is applied.
	 */
	virtual void SetFeedback21(uint8 StartZones, uint8 Amplitude, const EControllerHand& Hand) = 0;
	/**
	 * Configures the bow tension effect on the gamepad triggers based on specified parameters.
	 *
	 * @param StartZone The start position of the effect's activation zone on the trigger.
	 * @param SnapBack The intensity or force of the bow effect applied to the trigger.
	 * @param Hand The controller hand (left or right) to which the effect should be applied.
	 */
	virtual void SetBow22(uint8 StartZone, uint8 SnapBack, const EControllerHand& Hand) = 0;
	/**
	 * Configures a galloping effect on the gamepad with specified parameters for position, foot rhythm, and frequency.
	 *
	 * @param StartPosition The starting position of the galloping effect.
	 * @param EndPosition The ending position of the galloping effect.
	 * @param FirstFoot The force or intensity value assigned to the first foot.
	 * @param SecondFoot The force or intensity value assigned to the second foot.
	 * @param Frequency The frequency of the galloping effect.
	 * @param Hand The specific controller hand to which the effect is applied.
	 */
	virtual void SetGalloping23(uint8 StartPosition, uint8 EndPosition, uint8 FirstFoot, uint8 SecondFoot, uint8 Frequency, const EControllerHand& Hand) = 0;
	/**
	 * Configures trigger behavior for the specified weapon mode on a gamepad.
	 *
	 * @param StartZone The starting position of the trigger's actuation zone.
	 * @param Amplitude The intensity of the trigger's feedback effect.
	 * @param Behavior The type of feedback behavior applied to the trigger.
	 * @param Trigger The specific trigger to configure.
	 * @param Hand The controller hand (left or right) associated with the trigger.
	 */
	virtual void SetWeapon25(uint8 StartZone, uint8 Amplitude, uint8 Behavior, uint8 Trigger, const EControllerHand& Hand) = 0;
	/**
	 * Configures the machine gun effect on the gamepad triggers with specific parameters.
	 *
	 * @param StartZone The starting position of the trigger's activation zone for the machine gun effect.
	 * @param Behavior The behavior pattern or mode applied to the trigger.
	 * @param Amplitude The intensity level of the feedback effect on the trigger.
	 * @param Frequency The frequency of the feedback effect for the machine gun effect.
	 * @param Hand The controller hand (left or right) to which the machine gun effect is applied.
	 */
	virtual void SetMachineGun26(uint8 StartZone, uint8 Behavior, uint8 Amplitude, uint8 Frequency, const EControllerHand& Hand) = 0;
	/**
	 * Configures the behavior and haptic effects for a specific machine profile on a gamepad.
	 *
	 * @param StartZone The starting zone value indicating the trigger's activation point.
	 * @param BehaviorFlag A flag that determines the behavior pattern configuration for the trigger.
	 * @param Force The amount of force applied to the trigger feedback mechanism.
	 * @param Amplitude The amplitude level for the feedback intensity.
	 * @param Period The duration of the haptic feedback signal in each cycle.
	 * @param Frequency The frequency of the haptic feedback signal.
	 * @param Hand The hand (left or right) to which the feedback effect will be applied, based on the EControllerHand enumeration.
	 */
	virtual void SetMachine27(uint8 StartZone, uint8 BehaviorFlag, uint8 Force, uint8 Amplitude, uint8 Period, uint8 Frequency, const EControllerHand& Hand) = 0;
};
