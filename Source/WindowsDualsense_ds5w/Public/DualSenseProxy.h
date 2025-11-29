// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "Core/Enums/EDeviceCommons.h"
#include "Core/HapticsRegistry.h"
#include "Core/Structs/DualSenseFeatureReport.h"
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "SonyGamepadProxy.h"
#include "DualSenseProxy.generated.h"

/**
 * @brief Proxy class for PlayStation DualSense controller interactions and effects.
 *
 * Provides static functions to control various features and settings of a connected
 * PlayStation DualSense controller such as LED settings, vibration, trigger effects,
 * and connectivity status.
 */
UCLASS(Blueprintable, BlueprintType)
class WINDOWSDUALSENSE_DS5W_API UDualSenseProxy : public USonyGamepadProxy
{
	GENERATED_BODY()

public:
	/**
	 * Updates the settings for a DualSense controller using its feature report.
	 *
	 * @param ControllerId The ID of the controller to be updated.
	 * @param Settings The settings to be applied to the controller, encapsulated in an FDualSenseFeatureReport structure.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Settings")
	static void DeviceSettings(int32 ControllerId, FDualSenseFeatureReport Settings);
	/**
	 * @brief Registers a specific audio submix for a DualSense controller.
	 *
	 * Links a given sound submix to a connected PlayStation DualSense controller, identified by ControllerId.
	 * Enables audio haptics functionality to synchronize sound experiences with the controller's haptics capability.
	 *
	 * @param ControllerId The identifier for the connected DualSense controller.
	 * @param Submix The audio submix to be registered for haptic feedback on the controller.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense|Audio (Legacy)", meta = (DisplayName = "Register Submix Deprecated v1.2.20"))
	static void RegisterSubmixForDevice(int32 ControllerId, USoundSubmix* Submix);
	/**
	 * @brief Unregisters a submix listener associated with the specified DualSense controller device.
	 *
	 * This function removes the haptics listener for the specified controller ID, ensuring
	 * that any previously registered submix for audio or haptic feedback is no longer active.
	 *
	 * @param ControllerId The ID of the DualSense controller for which the submix listener will be unregistered.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense|Audio", meta = (DisplayName = "Unregister Submix Deprecated v1.2.20"))
	static void UnregisterSubmixForDevice(int32 ControllerId);
	/**
	 * @brief Activates an automatic gun effect on a specified DualSense controller.
	 *
	 * Simulates the behavior of an automatic firearm by adjusting trigger tension
	 * dynamically based on the provided strength parameters, hand specification,
	 * and other effect controls.
	 *
	 * @param ControllerId The ID of the controller to apply the effect on.
	 * @param BeginStrength The initial strength of the trigger effect. Ranges from 0 to 8.
	 * @param MiddleStrength The middle strength level of the trigger effect. Ranges from 0 to 8.
	 * @param EndStrength The final strength of the trigger effect. Ranges from 0 to 8.
	 * @param Hand The hand (left or right) to which the effect is applied.
	 * @param KeepEffect If true, maintains the effect even after the input stops; otherwise, stops once input ceases.
	 * @param Frequency The rate or frequency of the automatic gun effect.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects (Legacy)", meta = (DisplayName = "AutomaticGun Deprecated v1.2.20"))
	static void AutomaticGun(
	    int32 ControllerId,
	    UPARAM(DisplayName = "Begin Strength min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 BeginStrength,
	    UPARAM(DisplayName = "Middle Strength min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 MiddleStrength,
	    UPARAM(DisplayName = "End Strength min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 EndStrength,
	    EControllerHand Hand,
	    bool KeepEffect,
	    float Frequency = 5.0f);

	/**
	 * @brief Sets the GameCube trigger effect for the specified controller and hand.
	 *
	 * This method applies the preconfigured GameCube trigger effect to a connected DualSense
	 * controller using the specified controller ID and hand designation.
	 *
	 * @param ControllerId The identifier for the DualSense controller to target.
	 * @param Hand Specifies which hand (left or right) the effect should be applied to.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects (Legacy) Game Cube", meta = (DisplayName = "GameCube Deprecated v1.2.20"))
	static void GameCube(
	    int32 ControllerId,
	    EControllerHand Hand);

	/**
	 * @brief Configures custom trigger effects for a PlayStation DualSense controller.
	 *
	 * This method allows customization of the trigger input by defining a sequence of hexadecimal
	 * byte strings. It communicates with the controller to apply the specified trigger effect patterns.
	 *
	 * @param ControllerId The ID of the controller to configure.
	 * @param Hand Specifies which hand (left or right) the controller is associated with.
	 * @param HexBytes An array of hexadecimal byte strings representing the desired trigger configuration.
	 *                 Maximum of 10 byte strings is permitted.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects (Legacy)", meta = (DisplayName = "CustomTrigger Deprecated v1.2.20"))
	static void CustomTrigger(
	    int32 ControllerId,
	    EControllerHand Hand,
	    const TArray<FString>& HexBytes);

	/**
	 * Sets haptic feedback for a DualSense controller.
	 *
	 * @param ControllerId The ID of the controller for which feedback is to be set.
	 * @param BeginStrength The strength of the feedback at the beginning.
	 * @param MiddleStrength The strength of the feedback in the middle.
	 * @param EndStrength The strength of the feedback at the end.
	 * @param Hand The hand (left or right) associated with the controller.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects (Legacy)", meta = (DisplayName = "SetFeedback Deprecated v1.2.20"))
	static void SetFeedback(
	    int32 ControllerId,
	    UPARAM(DisplayName = "Begin Strength min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 BeginStrength,
	    UPARAM(DisplayName = "Middle Strength min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 MiddleStrength,
	    UPARAM(DisplayName = "End Strength min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 EndStrength,
	    EControllerHand Hand);

	/**
	 * Applies a resistance effect to the trigger of a PlayStation DualSense controller.
	 *
	 * This method configures the specified trigger to provide resistance feedback based on
	 * the given start position, end position, and strength parameters. The effect is applied
	 * to the controller defined by the ControllerId and the trigger specified using the Hand parameter.
	 *
	 * @param ControllerId The identifier of the controller to apply the resistance effect.
	 * @param StartPosition The starting position of the resistance zone within the trigger's range. Value must be between 0 and 8.
	 * @param EndPosition The ending position of the resistance zone within the trigger's range. Value must be between 0 and 8.
	 * @param Strength The strength of the resistance effect. Value must be between 0 and 8, with higher values indicating stronger resistance.
	 * @param Hand The trigger to apply the effect to, specified as EControllerHand (e.g., Left or Right).
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects (Legacy)", meta = (DisplayName = "Resistance Deprecated v1.2.20"))
	static void Resistance(
	    int32 ControllerId,
	    UPARAM(DisplayName = "Start Position min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 StartPosition,
	    UPARAM(DisplayName = "End Position min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 EndPosition,
	    UPARAM(DisplayName = "Strength min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 Strength,
	    EControllerHand Hand);

	/**
	 * Applies a continuous resistance effect on the adaptive trigger of a DualSense controller.
	 *
	 * @param ControllerId The ID of the controller to apply the effect to.
	 * @param StartPosition The starting position of the resistance effect. Valid range determined by internal validation.
	 * @param Strength The intensity of the resistance effect. Valid range determined by internal validation.
	 * @param Hand The controller hand (e.g., left or right) to which the resistance effect applies.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects (Legacy)", meta = (DisplayName = "ContinuousResistance Deprecated v1.2.20"))
	static void ContinuousResistance(
	    int32 ControllerId,
	    UPARAM(DisplayName = "Start Position min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 StartPosition,
	    UPARAM(DisplayName = "Strength min: 0 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 Strength,

	    EControllerHand Hand);

	/**
	 * Configures the bow effect on a DualSense controller.
	 *
	 * @param ControllerId The ID of the controller to configure the effect on.
	 * @param StartPosition The starting position of the trigger effect.
	 * @param EndPosition The ending position of the trigger effect.
	 * @param BeginStrength The strength of the trigger's resistance at the starting position.
	 * @param EndStrength The strength of the trigger's resistance at the ending position.
	 * @param Hand The hand (left or right) where the effect will be applied.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects (Legacy)", meta = (DisplayName = "Bow Deprecated v1.2.20"))
	static void Bow(
	    int32 ControllerId,
	    UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 StartPosition,
	    UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 EndPosition,
	    UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 BeginStrength,
	    UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 EndStrength,
	    EControllerHand Hand);

	/**
	 * @brief Triggers a galloping vibration effect on a DualSense controller.
	 *
	 * This function sets up and activates a galloping vibration pattern by defining
	 * the movement positions, associated vibration intensity, frequency, and the hand
	 * where the effect plays.
	 *
	 * @param ControllerId The ID of the controller to apply the effect.
	 * @param StartPosition The initial position of the galloping effect.
	 * @param EndPosition The final position of the galloping effect.
	 * @param FirstFoot The intensity for the first "foot" step in the galloping effect.
	 * @param SecondFoot The intensity for the second "foot" step in the galloping effect.
	 * @param Frequency The frequency at which the galloping effect repeats.
	 * @param Hand Specifies whether the effect is applied to the left or right hand.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects (Legacy)", meta = (DisplayName = "Galloping Deprecated v1.2.20"))
	static void Galloping(
	    int32 ControllerId,
	    UPARAM(meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 StartPosition,
	    UPARAM(meta = (ClampMin = "0", ClampMax = "9", UIMin = "0", UIMax = "9"))
	        int32 EndPosition,
	    UPARAM(DisplayName = "First Foot min: 2 max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 FirstFoot,
	    UPARAM(DisplayName = "Second Foot min: (Greater FirstFoot) max: 9", meta = (ClampMin = "0", ClampMax = "9", UIMin = "0", UIMax = "9"))
	        int32 SecondFoot,
	    UPARAM(DisplayName = "Frequency Example: 5.0", meta = (ClampMin = "0.0", ClampMax = "40.0", UIMin = "0.0", UIMax = "40.0")) float Frequency,
	    EControllerHand Hand);

	/**
	 * Novo efeito Machine avançado (opcode 0x27).
	 * Estrutura: [27] [Start_Zone] [Behavior_Flag] [Force_Amplitude] [Period] [Frequency]
	 * Start_Zone: ETriggerPosition -> mapeado para 0x82, 0x84, 0x80, 0x88
	 * Behavior_Flag: Localized=1 (EndAtPos), Sustained=2 (KeepEffect)
	 * Force_Amplitude: High nibble (1-3) = ETriggerForceIntensity, Low nibble (10-15) = EDualSenseTriggerAmplitude
	 * Period: 0-20, Frequency: 0-40
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects (Legacy)", meta = (DisplayName = "Machine Advanced (0x27)"))
	static void MachineAdvanced(
	    int32 ControllerId,
	    ETriggerPosition StartZone,
	    ETriggerEffectBehavior Behavior,
	    ETriggerForceIntensity ForceIntensity,
	    EDualSenseTriggerAmplitude Amplitude,
	    UPARAM(meta = (ClampMin = "0", ClampMax = "20", UIMin = "0", UIMax = "20"))
	        int32 Period,
	    UPARAM(meta = (ClampMin = "0", ClampMax = "40", UIMin = "0", UIMax = "40"))
	        int32 Frequency,
	    EControllerHand Hand);

	/**
	 * Configures a weapon effect on the DualSense controller using specified parameters.
	 *
	 * @param ControllerId The identifier for the target DualSense controller.
	 * @param StartPosition The starting position of the effect in the trigger. The value should be validated and within the range of allowed positions.
	 * @param EndPosition The ending position of the effect in the trigger. The value should be validated and within the range of allowed positions.
	 * @param Strength The strength of the weapon effect. The value should be validated and within the range of allowed strengths.
	 * @param Hand Specifies which controller hand (left or right) should be affected by the weapon effect.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Effects (Legacy)", meta = (DisplayName = "Deprecated v1.2.20"))
	static void Weapon(
	    int32 ControllerId,
	    UPARAM(DisplayName = "Start Position min: 2", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 StartPosition,
	    UPARAM(DisplayName = "End Position max: 7", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 EndPosition,
	    UPARAM(DisplayName = "Strength max: 8", meta = (ClampMin = "0", ClampMax = "8", UIMin = "0", UIMax = "8"))
	        int32 Strength,
	    EControllerHand Hand);

	/**
	 * Controls the LED player light effects on the DualSense controller.
	 *
	 * @param ControllerId The identifier for the target controller.
	 * @param Value The LED pattern enum specifying the LED configuration for the player indicator (e.g., Off, Player One, Player Two).
	 * @param Brightness The brightness level of the LED lights specified by an enum (e.g., Low, Medium, High).
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Led Effects (Legacy)", meta = (DisplayName = "Deprecated v1.2.20"))
	static void LedPlayerEffects(int32 ControllerId, ELedPlayerEnum Value, ELedBrightnessEnum Brightness);

	/**
	 * Disables the resistance effect for the specified controller and hand.
	 *
	 * @param ControllerId The unique identifier for the DualSense controller.
	 * @param Hand The hand (left or right) associated with the effect to disable resistance for.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Reset Effects (Legacy)", meta = (DisplayName = "Deprecated v1.2.20"))
	static void NoResistance(int32 ControllerId, EControllerHand Hand);

	/**
	 * Stops the trigger effect on a specific controller for the specified hand.
	 *
	 * @param ControllerId The unique identifier of the controller for which the trigger effect should be stopped.
	 * @param HandStop Specifies which hand's trigger effect (left or right) should be stopped.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Reset Effects (Legacy)", meta = (DisplayName = "Deprecated v1.2.20"))
	static void StopTriggerEffect(int32 ControllerId, EControllerHand HandStop);

	/**
	 * Stops all trigger effects currently active for the specified DualSense controller.
	 *
	 * @param ControllerId The ID of the DualSense controller for which to stop all trigger effects.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Reset Effects")
	static void StopAllTriggersEffects(int32 ControllerId);

	/**
	 * Resets all haptic feedback effects for the specified DualSense controller.
	 *
	 * This method stops all active effects currently running on the specified DualSense controller.
	 *
	 * @param ControllerId The identifier of the controller whose effects are to be reset.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Reset Effects")
	static void ResetEffects(int32 ControllerId);

	/**
	 * Retrieves the strength of the right trigger feedback for the specified controller.
	 *
	 * @param ControllerId The unique identifier for the controller whose right trigger feedback strength is to be retrieved.
	 * @return The strength value of the right trigger feedback. Returns 0 if the controller instance is not found.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Status")
	static int32 GetTriggerRightStrengthFeedback(int32 ControllerId)
	{
		return 0;
	}

	/**
	 * Retrieves the strength of the left trigger feedback for the specified DualSense controller.
	 *
	 * @param ControllerId The ID of the controller to retrieve the feedback strength for.
	 * @return The strength of the left trigger feedback as an integer. Returns 0 if the controller instance is not found.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualSense Status")
	int32 GetTriggerLeftStrengthFeedback(int32 ControllerId)
	{
		return 0;
	}
};
