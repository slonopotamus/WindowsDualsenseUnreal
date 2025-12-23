// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "DeviceManager.h"
#include "API/SonyGamepadProxyHelpers.h"
#include "API/SonyGamepadSettingsProxy.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "GCore/Types/DSCoreTypes.h"
#include "GCore/Types/ECoreGamepad.h"
#include "GenericPlatform/IInputInterface.h"
#include "IInputDevice.h"
#include "InputCoreTypes.h"
#include "Misc/CoreDelegates.h"
#include "Runtime/Launch/Resources/Version.h"

using namespace DSCoreTypes;
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

void DeviceManager::Tick(float DeltaTime)
{
	FDeviceRegistry* Registry = FDeviceRegistry::Get();
	if (Registry)
	{
		Registry->DiscoverDevices(DeltaTime);
	}

	PollAccumulator += DeltaTime;
	if (PollAccumulator < PluginSettings::PollInterval)
	{
		return;
	}
	PollAccumulator = 0.0f;

	TArray<FInputDeviceId> OutInputDevices;
	OutInputDevices.Reset();
	IPlatformInputDeviceMapper::Get().GetAllConnectedInputDevices(OutInputDevices);
	for (const FInputDeviceId& Device : OutInputDevices)
	{
		if (ISonyGamepad* Gamepad = Registry->GetLibraryInstance(Device); Gamepad)
		{
			const FPlatformUserId UserId = IPlatformInputDeviceMapper::Get().GetUserForInputDevice(Device);
			if (const int32 ControllerId = FPlatformMisc::GetUserIndexForPlatformUser(UserId); ControllerId == -1)
			{
				continue;
			}

			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [=]() {
				if (ISonyGamepad* Ref = Registry->GetLibraryInstance(Device); Ref)
				{
					Ref->UpdateInput(PluginSettings::PollInterval);
				}
			});
		}
	}

	SendControllerEvents(PluginSettings::PollInterval);
}

void DeviceManager::SendControllerEvents(float DeltaTime)
{
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
			if (Gamepad->GetDeviceType() == EDSDeviceType::DualShock4)
			{
				ContextDrive = TEXT("DualShock4");
			}
			if (Gamepad->GetDeviceType() == EDSDeviceType::DualSenseEdge)
			{
				ContextDrive = TEXT("DualSenseEdge");
			}
			FInputDeviceScope InputScope(this, TEXT("DeviceManager.WindowsDualsense"), Device.GetId(), ContextDrive);
			if (FDeviceContext* Context = Gamepad->GetMutableDeviceContext())
			{
				FInputContext FrameInput = Context->GetInputState();
				CheckEvents(Context, FrameInput, UserId, Device, DeltaTime);
			}
		}
	}
}

namespace TouchGestureDirectRaw
{
	constexpr float DirectionRawDegreesOffset = 0.0f;

	constexpr float DirectionFlipX = 1.0f;
	constexpr float DirectionFlipY = 1.0f;

	static FVector2D DirectionRawToUnitVector(std::uint8_t Raw)
	{
		const float Degrees = (static_cast<float>(Raw) * (360.0f / 256.0f)) + DirectionRawDegreesOffset;
		const float Radians = FMath::DegreesToRadians(Degrees);

		FVector2D Dir(FMath::Cos(Radians), FMath::Sin(Radians));
		Dir.X *= DirectionFlipX;
		Dir.Y *= DirectionFlipY;

		return Dir.GetSafeNormal();
	}
} // namespace TouchGestureDirectRaw

void DeviceManager::CheckEvents(FDeviceContext* Context, FInputContext& FrameInput, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, float DeltaTime) const
{
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::LeftStickLeft, FrameInput.bLeftAnalogLeft);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::LeftStickRight, FrameInput.bLeftAnalogRight);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::LeftStickDown, FrameInput.bLeftAnalogDown);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::LeftStickUp, FrameInput.bLeftAnalogUp);

	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::RightStickLeft, FrameInput.bRightAnalogLeft);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::RightStickRight, FrameInput.bRightAnalogRight);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::RightStickDown, FrameInput.bRightAnalogDown);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::RightStickUp, FrameInput.bRightAnalogUp);

	MessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogX, UserId, InputDeviceId, FrameInput.LeftAnalog.X);
	MessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogY, UserId, InputDeviceId, FrameInput.LeftAnalog.Y);
	MessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogX, UserId, InputDeviceId, FrameInput.RightAnalog.X);
	MessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogY, UserId, InputDeviceId, FrameInput.RightAnalog.Y);
	
	MessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftTriggerAnalog, UserId, InputDeviceId, FrameInput.LeftTriggerAnalog);
	MessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightTriggerAnalog, UserId, InputDeviceId, FrameInput.RightTriggerAnalog);

	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonBottom, FrameInput.bCross);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonLeft, FrameInput.bSquare);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonRight, FrameInput.bCircle);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonTop, FrameInput.bTriangle);

	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::DPadUp, FrameInput.bDpadUp);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::DPadDown, FrameInput.bDpadDown);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::DPadLeft, FrameInput.bDpadLeft);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::DPadRight, FrameInput.bDpadRight);

	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::LeftShoulder, FrameInput.bLeftShoulder);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::RightShoulder, FrameInput.bRightShoulder);

	// mapped urenal native gamepad Start and Select
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::SpecialRight, FrameInput.bStart);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::SpecialLeft, FrameInput.bShare);

	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::LeftTriggerThreshold, FrameInput.bLeftTriggerThreshold);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::RightTriggerThreshold, FrameInput.bRightTriggerThreshold);

	// mapped urenal native gamepad Push Stick
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::LeftThumb, FrameInput.bLeftStick);
	CheckButtonInput(Context, UserId, InputDeviceId, FGamepadKeyNames::RightThumb, FrameInput.bRightStick);

	// Custom map keys
	CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_PushLeftStick"), FrameInput.bLeftStick);
	CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_PushRightStick"), FrameInput.bRightStick);

	CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_Mic"), FrameInput.bMute);
	CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_TouchButtom"), FrameInput.bTouch);
	CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_Button"), FrameInput.bPSButton);

	if (Context->DeviceType == EDSDeviceType::DualSenseEdge)
	{
		CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_FunctionL"), FrameInput.bFn1);
		CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_FunctionR"), FrameInput.bFn2);
		CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_PaddleL"), FrameInput.bPaddleLeft);
		CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_PaddleR"), FrameInput.bPaddleRight);
	}

	CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_Menu"), FrameInput.bStart);
	CheckButtonInput(Context, UserId, InputDeviceId, FName("PS_Share"), FrameInput.bShare);

	if (Context->bEnableAccelerometerAndGyroscope)
	{
		constexpr float RadDeg = DS_RAD_TO_DEG;
		constexpr float GToMSq = GRAVITY_MS2;
		float RawGyroX = FrameInput.Gyroscope.X;
		float RawGyroY = FrameInput.Gyroscope.Y;
		float RawGyroZ = FrameInput.Gyroscope.Z;
		float RawAcclX = FrameInput.Accelerometer.X;
		float RawAcclY = FrameInput.Accelerometer.Y;
		float RawAcclZ = FrameInput.Accelerometer.Z;

		// Z from X, X from Y, Y from Z
		float G_Roll = RawGyroZ * RadDeg;
		float G_Pitch = RawGyroX * RadDeg;
		float G_Yaw = -RawGyroY * RadDeg;
		float A_Roll = RawAcclZ * GToMSq;
		float A_Pitch = RawAcclX * GToMSq;
		float A_Yaw = -RawAcclY * GToMSq;

		if (!FilterSensors.Contains(InputDeviceId))
		{
			TUniquePtr<FMadgwickAhrs> MadgwickAhrs = MakeUnique<FMadgwickAhrs>(PluginSettings::MadgwickBeta);
			FilterSensors.Add(InputDeviceId, MadgwickAhrs.Release());
		}

		if (Context->bIsResetGyroscope)
		{
			FilterSensors[InputDeviceId]->Reset();
			Context->bIsResetGyroscope = false;
		}

		FilterSensors[InputDeviceId]->UpdateImu(G_Roll, G_Pitch, G_Yaw, A_Roll, A_Pitch, A_Yaw, DeltaTime);

		float qw, qx, qy, qz;
		FilterSensors[InputDeviceId]->GetQuaternion(qw, qx, qy, qz);
		const FQuat RawSensorQuat = FQuat(qx, qy, qz, qw);
		FRotator TiltRotator = RawSensorQuat.Rotator();
		FVector UnrealGyro(G_Roll, G_Pitch, G_Yaw);
		FVector UnrealAccel(A_Roll, A_Pitch, A_Yaw);
		FVector UnrealTilt = FVector(TiltRotator.Roll, TiltRotator.Pitch, TiltRotator.Yaw);

		MessageHandler.Get().OnMotionDetected(UnrealTilt, UnrealGyro, RawSensorQuat.GetUpVector(), UnrealAccel, UserId, InputDeviceId);
	}
	
}

void DeviceManager::CheckButtonInput(FDeviceContext* Context, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, const FName ButtonName, const bool IsButtonPressed) const
{
	const std::string Str(TCHAR_TO_UTF8(*ButtonName.ToString()));
	const bool PreviousState = Context->ButtonStates[Str];
	if (IsButtonPressed && !PreviousState)
	{
		MessageHandler.Get().OnControllerButtonPressed(ButtonName, UserId, InputDeviceId, false);
	}

	if (!IsButtonPressed && PreviousState)
	{
		MessageHandler.Get().OnControllerButtonReleased(ButtonName, UserId, InputDeviceId, false);
	}

	Context->ButtonStates[Str] = IsButtonPressed;
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
				GamepadTrigger->SetResistance(FeedbackProperty->Position, FeedbackProperty->Strengh, static_cast<EDSGamepadHand>(HandMask));
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

				std::vector<uint8_t> StrBytes;
				StrBytes.push_back(0x21);
				StrBytes.push_back(RawPos);
				StrBytes.push_back(RawPos & 0xf0);
				StrBytes.push_back(0x03);
				StrBytes.push_back(Force >> 24 & 0xff);
				StrBytes.push_back(Force >> 16 & 0xff);
				StrBytes.push_back(Force >> 8 & 0xff);
				StrBytes.push_back(Force >> 0 & 0x3f);
				StrBytes.push_back(0);
				StrBytes.push_back(0);
				StrBytes.push_back(0);

				GamepadTrigger->SetCustomTrigger(static_cast<EDSGamepadHand>(HandMask), StrBytes);
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
		const float LeftRumble = FMath::Clamp(FMath::Max(Values.LeftLarge, Values.LeftSmall), 0.f, 1.f);
		const float RightRumble = FMath::Clamp(FMath::Max(Values.RightLarge, Values.RightSmall), 0.f, 1.f);
		Gamepad->SetVibration(static_cast<uint8>(LeftRumble * 255.f), static_cast<uint8>(RightRumble * 255.f));
	}
}

void DeviceManager::SetLightColor(const int32 ControllerId, const FColor Color)
{
	if (ISonyGamepad* Gamepad = GetGamepad(ControllerId))
	{
		Gamepad->SetLightbar({Color.R, Color.G, Color.B, Color.A});
	}
}

bool DeviceManager::IsGamepadAttached() const
{
	return true;
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
