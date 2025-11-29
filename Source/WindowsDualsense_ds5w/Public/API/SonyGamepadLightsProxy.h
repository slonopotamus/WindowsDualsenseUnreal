// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core/Types/Enums/EDeviceCommons.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SonyGamepadLightsProxy.generated.h"

/**
 *
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API USonyGamepadLightsProxy : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Sets the lightbar color on a controller.
	 *
	 * @param ControllerId The ID of the DualSense controller to be affected.
	 * @param Color The desired lightbar color, represented as an FColor value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepadLights", meta = (AdvancedDisplay = "Ligthbar Color (R,G,B)", ToolTip = "Sets the lightbar color on a controller."))
	static void Ligthbar(int32 ControllerId, FColor Color);
	/**
	 * Controls the LED and microphone visual effects on a DualSense controller.
	 *
	 * @param ControllerId The ID of the DualSense controller to be affected.
	 * @param Value The desired LED and microphone effect to apply, represented as an ELedMicEnum value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SonyGamepadLights", meta = (AdvancedDisplay = "Player Led (One, Two)"))
	static void PlayerLed(int32 ControllerId, ELedPlayerEnum Value, ELedBrightnessEnum Brightness);
};
