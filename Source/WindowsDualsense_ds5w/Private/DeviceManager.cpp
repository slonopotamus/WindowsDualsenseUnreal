// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "DeviceManager.h"
#include "API/SonyGamepadProxyHelpers.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Core/Managers/DeviceRegistry.h"
#include "Core/Types/Enums/EDeviceConnection.h"
#include "Misc/CoreDelegates.h"

using namespace SonyGamepadProxyHelpers;

DeviceManager::DeviceManager(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
    : MessageHandler(InMessageHandler)
{
	FCoreDelegates::OnUserLoginChangedEvent.AddRaw(this, &DeviceManager::OnUserLoginChangedEvent);
}

DeviceManager::~DeviceManager()
{
	FCoreDelegates::OnUserLoginChangedEvent.RemoveAll(this);
}

void DeviceManager::SendControllerEvents()
{
}

void DeviceManager::Tick(float DeltaTime)
{
	FDeviceRegistry::Get()->DetectedChangeConnections(DeltaTime);

	PollAccumulator += DeltaTime;
	if (PollAccumulator < PollInterval)
	{
		return;
	}

	PollAccumulator = 0.0f;

	TArray<FInputDeviceId> OutInputDevices;
	OutInputDevices.Reset();
	IPlatformInputDeviceMapper::Get().GetAllConnectedInputDevices(OutInputDevices);
	for (const FInputDeviceId& Device : OutInputDevices)
	{
		if (ISonyGamepad* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(Device); Gamepad)
		{
			const FPlatformUserId UserId = IPlatformInputDeviceMapper::Get().GetUserForInputDevice(Device);
			if (const int32 ControllerId = FPlatformMisc::GetUserIndexForPlatformUser(UserId); ControllerId == -1)
			{
				continue;
			}

			FString ContextDrive = TEXT("DualSense");
			if (Gamepad->GetDeviceType() == EDeviceType::DualShock4)
			{
				ContextDrive = TEXT("DualShock4");
			}
			if (Gamepad->GetDeviceType() == EDeviceType::DualSenseEdge)
			{
				ContextDrive = TEXT("DualSenseEdge");
			}

			FInputDeviceScope InputScope(this, TEXT("DeviceManager.WindowsDualsense"), Device.GetId(), ContextDrive);
			Gamepad->UpdateInput(MessageHandler, UserId, Device, DeltaTime);
		}
	}
}

void DeviceManager::SetDeviceProperty(int32 ControllerId, const FInputDeviceProperty* Property)
{
	if (!Property)
	{
		return;
	}

	if (Property->Name == FInputDeviceLightColorProperty::PropertyName())
	{
		const FInputDeviceLightColorProperty* ColorProperty = static_cast<const FInputDeviceLightColorProperty*>(Property);
		SetLightColor(ControllerId, ColorProperty->Color);
	}

	if (Property->Name == FInputDeviceTriggerFeedbackProperty::PropertyName())
	{
		if (const FInputDeviceTriggerFeedbackProperty* FeedbackProperty = static_cast<const FInputDeviceTriggerFeedbackProperty*>(Property))
		{
			EInputDeviceTriggerMask HandMask = FeedbackProperty->AffectedTriggers;
			if (IGamepadTrigger* GamepadTrigger = GetTriggerInterface(ControllerId))
			{
				GamepadTrigger->SetResistance(FeedbackProperty->Position, FeedbackProperty->Strengh, static_cast<EControllerHand>(HandMask));
			}
		}
	}

	if (Property->Name == FInputDeviceTriggerResistanceProperty::PropertyName())
	{
		if (const FInputDeviceTriggerResistanceProperty* FeedbackAdvancedProperty = static_cast<const FInputDeviceTriggerResistanceProperty*>(Property))
		{
			EInputDeviceTriggerMask HandMask = FeedbackAdvancedProperty->AffectedTriggers;
			if (IGamepadTrigger* GamepadTrigger = GetTriggerInterface(ControllerId))
			{
				TArray<FString> HexBytes;
				HexBytes.Reserve(10);

				const double P1 = (FMath::Clamp(FeedbackAdvancedProperty->StartPosition, 0.f, 9.f) / 9.f) * 1023.0f;
				const double P2 = (FMath::Clamp(FeedbackAdvancedProperty->EndPosition, P1, 9.f) / 9.f) * 1023.0f;
				const double F1 = (FMath::Clamp(FeedbackAdvancedProperty->StartStrengh, 0.f, 8.f) / 8.f) * 255.f;
				const double F2 = (FMath::Clamp(FeedbackAdvancedProperty->EndStrengh, 0.f, 8.f) / 8.f) * 255.f;

				const int PIndex = FeedbackAdvancedProperty->StartPosition;
				const int PIndexEnd = FeedbackAdvancedProperty->EndPosition;

				uint32 Force = 0;
				const double Slope = ((F2 - F1) / (P2 - P1));
				for (int i = PIndex; i < PIndexEnd; i++)
				{
					Force |= static_cast<uint32>((F1 + (Slope * i)) * 1023.0f);
				}

				uint8 RawPos;
				if (PIndex < 3)
				{
					RawPos = 255;
				}
				else if (PIndex < 5 && PIndex > 3)
				{
					RawPos = 230;
				}
				else
				{
					RawPos = 200;
				}

				HexBytes.Add("0x21");
				HexBytes.Add(FString::Printf(TEXT("0x%02X"), RawPos & 0xf0));
				HexBytes.Add(FString::Printf(TEXT("0x%02X"), 0x03));
				HexBytes.Add(FString::Printf(TEXT("0x%02X"), Force >> 24 & 0xff));
				HexBytes.Add(FString::Printf(TEXT("0x%02X"), Force >> 16 & 0xff));
				HexBytes.Add(FString::Printf(TEXT("0x%02X"), Force >> 8 & 0xff));
				HexBytes.Add(FString::Printf(TEXT("0x%02X"), Force >> 0 & 0x3f));
				HexBytes.Add("0x00");
				HexBytes.Add("0x00");
				HexBytes.Add("0x00");
				GamepadTrigger->SetCustomTrigger(static_cast<EControllerHand>(HandMask), HexBytes);
			}
		}
	}
}

void DeviceManager::SetHapticFeedbackValues(const int32 ControllerId, const int32 Hand, const FHapticFeedbackValues& Values)
{
}

void DeviceManager::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->SetVibration(Values);
	}
}

void DeviceManager::SetLightColor(const int32 ControllerId, const FColor Color)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->SetLightbar(Color);
	}
}

bool DeviceManager::IsGamepadAttached() const
{
	return FDeviceRegistry::Get()->GetAllocatedDevices() > 0;
}

void DeviceManager::OnUserLoginChangedEvent(bool bLoggedIn, int32 UserId, int32 UserIndex)
{
	const FPlatformUserId PlatformUserId = FPlatformUserId::CreateFromInternalId(UserId);
	if (!bLoggedIn)
	{
		TArray<FInputDeviceId> OutInputDevices;
		OutInputDevices.Reset();

		IPlatformInputDeviceMapper::Get().GetAllInputDevicesForUser(PlatformUserId, OutInputDevices);
		for (const FInputDeviceId& DeviceId : OutInputDevices)
		{
			IPlatformInputDeviceMapper::Get().Internal_MapInputDeviceToUser(DeviceId, PlatformUserId, EInputDeviceConnectionState::Disconnected);
		}
	}
}
