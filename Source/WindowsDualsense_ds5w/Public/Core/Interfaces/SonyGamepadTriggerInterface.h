// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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
};
