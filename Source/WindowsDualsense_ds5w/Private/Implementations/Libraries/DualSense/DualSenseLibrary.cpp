// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Implementations/Libraries/DualSense/DualSenseLibrary.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Core/Algorithms/MadgwickAhrs.h"
#include "Core/Interfaces/IPlatformHardwareInfo.h"
#include "Core/Types/Enums/EDeviceConnection.h"
#include "Core/Types/Structs/Context/DeviceContext.h"
#include "Helpers/ValidateHelpers.h"
#include "Implementations/Utils/DualSenseTriggerComposer.h"
#include "Implementations/Utils/GamepadCalibrationSensors.h"
#include "Implementations/Utils/PlayStationOutputComposer.h"
#include "InputCoreTypes.h"

using namespace DualSenseTriggerComposer;

void FDualSenseLibrary::SetVibration(const FForceFeedbackValues& Values)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	if (!Context)
	{
		return;
	}

	FOutputContext* HidOutput = &Context->Output;
	const float LeftRumble = FMath::Max(Values.LeftLarge, Values.LeftSmall);
	const float RightRumble = FMath::Max(Values.RightLarge, Values.RightSmall);

	const unsigned char OutputLeft = static_cast<unsigned char>(FValidateHelpers::To255(LeftRumble));
	const unsigned char OutputRight = static_cast<unsigned char>(FValidateHelpers::To255(RightRumble));
	if (HidOutput->Rumbles.Left != OutputLeft || HidOutput->Rumbles.Right != OutputRight)
	{
		HidOutput->Rumbles = {OutputLeft, OutputRight};
		UpdateOutput();
	}
}

void FDualSenseLibrary::ResetLights()
{
	FDeviceContext* Context = GetMutableDeviceContext();
	if (!Context)
	{
		return;
	}

	FOutputContext* HidOutput = &Context->Output;
	if (HidOutput->Lightbar.A == 0 && HidOutput->Lightbar.B == 0 && HidOutput->Lightbar.R == 0)
	{
		HidOutput->Lightbar.B = 255;
	}

	HidOutput->PlayerLed.Led = static_cast<unsigned char>(ELedPlayerEnum::One);
	UpdateOutput();
}

void FDualSenseLibrary::SetLightbar(FColor Color, float BrithnessTime, float ToggleTime)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	if (!Context)
	{
		return;
	}

	FOutputContext* HidOutput = &Context->Output;
	if ((HidOutput->Lightbar.R != Color.R) || (HidOutput->Lightbar.G != Color.G) || (HidOutput->Lightbar.B != Color.B))
	{
		HidOutput->Lightbar.R = Color.R;
		HidOutput->Lightbar.G = Color.G;
		HidOutput->Lightbar.B = Color.B;
		UpdateOutput();
	}
}

bool FDualSenseLibrary::Initialize(const FDeviceContext& Context)
{
	SetDeviceContexts(Context);
	FDeviceContext* DSContext = GetMutableDeviceContext();
	if (DSContext->ConnectionType == EDeviceConnection::Bluetooth)
	{
		FOutputContext* EnableReport = &DSContext->Output;
		// Set flags to enable control over the lightbar, player LEDs
		EnableReport->Feature.FeatureMode = 0x55;
		EnableReport->Lightbar = {0, 0, 222};
		EnableReport->PlayerLed.Brightness = 0x00;
		UpdateOutput();

		FPlatformProcess::Sleep(0.1f);
		DSContext->BufferAudio[0] = 0x32;
		DSContext->BufferAudio[1] = 0x00;
		DSContext->BufferAudio[2] = 0x91;
		DSContext->BufferAudio[3] = 0x07;
		DSContext->BufferAudio[4] = 0xFE;
		DSContext->BufferAudio[5] = 55;
		DSContext->BufferAudio[6] = 55;
		DSContext->BufferAudio[7] = 15;
		DSContext->BufferAudio[8] = 50;
		DSContext->BufferAudio[9] = 50;
	}

	ResetLights();
	return true;
}

void FDualSenseLibrary::Settings(const FDualSenseFeatureReport& Settings)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	if (Settings.VibrationMode == EDualSenseDeviceFeatureReport::Off)
	{
		Context->Output.Feature.VibrationMode = 0xFC;
	}

	Context->Output.Feature.SoftRumbleReduce = static_cast<uint8>(Settings.SoftRumbleReduce);
	Context->Output.Feature.TriggerSoftnessLevel = static_cast<uint8>(Settings.TriggerSoftnessLevel);
	Context->Output.Audio.MicStatus = static_cast<uint8>(Settings.MicStatus);
	Context->Output.Audio.MicVolume = static_cast<uint8>(Settings.MicVolume);
	Context->Output.Audio.HeadsetVolume = static_cast<uint8>(Settings.AudioVolume);
	Context->Output.Audio.SpeakerVolume = static_cast<uint8>(Settings.AudioVolume);
	Context->Output.Audio.Mode = 0x08;
	if (Settings.AudioHeadset == EDualSenseAudioFeatureReport::On && Settings.AudioSpeaker == EDualSenseAudioFeatureReport::Off)
	{
		Context->Output.Audio.Mode = 0x31;
	}

	if (Settings.AudioHeadset == EDualSenseAudioFeatureReport::Off && Settings.AudioSpeaker == EDualSenseAudioFeatureReport::On)
	{
		Context->Output.Audio.Mode = 0x21;
	}
	UpdateOutput();
}

void FDualSenseLibrary::UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
                                    const FPlatformUserId UserId, const FInputDeviceId InputDeviceId, float Delta)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [NewContext = MoveTemp(Context)]() {
		IPlatformHardwareInfo::Get().Read(NewContext);
	});

	const size_t Padding = Context->ConnectionType == EDeviceConnection::Bluetooth ? 2 : 1;
	const unsigned char* HIDInput = &Context->Buffer[Padding];

	const auto HandleAnalogInput = [&](const FName& AnalogKey, const FName& ButtonKeyPositive, const FName& ButtonKeyNegative, float NewAxisValue) {
		if (FMath::Abs(NewAxisValue) < GetAnalogDeadZone())
		{
			NewAxisValue = 0;
		}

		auto& OldAxisValue = AnalogStates.FindOrAdd(AnalogKey);

		if (FMath::IsNearlyEqual(NewAxisValue, OldAxisValue))
		{
			return;
		}

		InMessageHandler->OnControllerAnalog(AnalogKey, UserId, InputDeviceId, NewAxisValue);
		OldAxisValue = NewAxisValue;

		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, ButtonKeyPositive, NewAxisValue > 0);
		CheckButtonInput(InMessageHandler, UserId, InputDeviceId, ButtonKeyNegative, NewAxisValue < 0);
	};

	// Analogs
	const float LeftAnalogX = static_cast<float>(HIDInput[0x00] - 128) / 128;
	const float LeftAnalogY = static_cast<float>(HIDInput[0x01] - 128) / -128;
	const float RightAnalogX = static_cast<float>(HIDInput[0x02] - 128) / 128;
	const float RightAnalogY = static_cast<float>(HIDInput[0x03] - 128) / -128;

	HandleAnalogInput(FGamepadKeyNames::LeftAnalogX, FGamepadKeyNames::LeftStickLeft, FGamepadKeyNames::LeftStickRight, LeftAnalogX);
	HandleAnalogInput(FGamepadKeyNames::LeftAnalogY, FGamepadKeyNames::LeftStickDown, FGamepadKeyNames::LeftStickUp, LeftAnalogY);
	HandleAnalogInput(FGamepadKeyNames::RightAnalogX, FGamepadKeyNames::RightStickLeft, FGamepadKeyNames::RightStickRight, RightAnalogX);
	HandleAnalogInput(FGamepadKeyNames::RightAnalogY, FGamepadKeyNames::RightStickDown, FGamepadKeyNames::RightStickUp, RightAnalogY);

	const float TriggerL = HIDInput[0x04] / 256.0f;
	const float TriggerR = HIDInput[0x05] / 256.0f;
	InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::LeftTriggerAnalog, UserId, InputDeviceId, TriggerL);
	InMessageHandler.Get().OnControllerAnalog(FGamepadKeyNames::RightTriggerAnalog, UserId, InputDeviceId, TriggerR);

	uint8_t ButtonsMask = HIDInput[0x07] & 0xF0;
	const bool bCross = ButtonsMask & BTN_CROSS;
	const bool bSquare = ButtonsMask & BTN_SQUARE;
	const bool bCircle = ButtonsMask & BTN_CIRCLE;
	const bool bTriangle = ButtonsMask & BTN_TRIANGLE;

	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonBottom, bCross);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonLeft, bSquare);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonRight, bCircle);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::FaceButtonTop, bTriangle);

	switch (HIDInput[0x07] & 0x0F)
	{
		case 0x0:
			ButtonsMask |= BTN_DPAD_UP;
			break;
		case 0x4:
			ButtonsMask |= BTN_DPAD_DOWN;
			break;
		case 0x6:
			ButtonsMask |= BTN_DPAD_LEFT;
			break;
		case 0x2:
			ButtonsMask |= BTN_DPAD_RIGHT;
			break;
		case 0x5:
			ButtonsMask |= BTN_DPAD_LEFT | BTN_DPAD_DOWN;
			break;
		case 0x7:
			ButtonsMask |= BTN_DPAD_LEFT | BTN_DPAD_UP;
			break;
		case 0x1:
			ButtonsMask |= BTN_DPAD_RIGHT | BTN_DPAD_UP;
			break;
		case 0x3:
			ButtonsMask |= BTN_DPAD_RIGHT | BTN_DPAD_DOWN;
			break;
		default:;
	}
	const bool bDPadLeft = ButtonsMask & BTN_DPAD_LEFT;
	const bool bDPadDown = ButtonsMask & BTN_DPAD_DOWN;
	const bool bDPadRight = ButtonsMask & BTN_DPAD_RIGHT;
	const bool bDPadUp = ButtonsMask & BTN_DPAD_UP;

	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadUp, bDPadUp);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadDown, bDPadDown);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadLeft, bDPadLeft);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::DPadRight, bDPadRight);

	// Shoulders
	const bool bLeftShoulder = HIDInput[0x08] & BTN_LEFT_SHOLDER;
	const bool bRightShoulder = HIDInput[0x08] & BTN_RIGHT_SHOLDER;

	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftShoulder, bLeftShoulder);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightShoulder, bRightShoulder);

	// Push Stick
	const bool PushLeftStick = HIDInput[0x08] & BTN_LEFT_STICK;
	const bool PushRightStick = HIDInput[0x08] & BTN_RIGHT_STICK;
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PushLeftStick"), PushLeftStick);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PushRightStick"), PushRightStick);

	// mapped urenal native gamepad Push Stick
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftThumb, PushLeftStick);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightThumb, PushRightStick);

	// Function & Special Actions
	const bool Playstation = HIDInput[0x09] & BTN_PLAYSTATION_LOGO;
	const bool TouchPad = HIDInput[0x09] & BTN_PAD_BUTTON;
	const bool Mic = HIDInput[0x09] & BTN_MIC_BUTTON;
	const bool bFn1 = HIDInput[0x09] & BTN_FN1;
	const bool bFn2 = HIDInput[0x09] & BTN_FN2;
	const bool bPaddleLeft = HIDInput[0x09] & BTN_PADDLE_LEFT;
	const bool bPaddleRight = HIDInput[0x09] & BTN_PADDLE_RIGHT;

	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Mic"), Mic);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_TouchButtom"), TouchPad);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Button"), Playstation);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_FunctionL"), bFn1);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_FunctionR"), bFn2);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PaddleL"), bPaddleLeft);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_PaddleR"), bPaddleRight);

	const bool Start = HIDInput[0x08] & BTN_START;
	const bool Select = HIDInput[0x08] & BTN_SELECT;
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Menu"), Start);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FName("PS_Share"), Select);

	// mapped urenal native gamepad Start and Select
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::SpecialRight, Start);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::SpecialLeft, Select);

	const bool bLeftTriggerThreshold = HIDInput[0x08] & BTN_LEFT_TRIGGER;
	const bool bRightTriggerThreshold = HIDInput[0x08] & BTN_RIGHT_TRIGGER;
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::LeftTriggerThreshold,
	                 bLeftTriggerThreshold);
	CheckButtonInput(InMessageHandler, UserId, InputDeviceId, FGamepadKeyNames::RightTriggerThreshold,
	                 bRightTriggerThreshold);
	if (IsEnableTouch())
	{
		FTouchPoint1 Touch;
		const int32 Touchpad1Raw = *reinterpret_cast<const int32*>(&HIDInput[0x20]);
		Touch.Y = (Touchpad1Raw & 0xFFF00000) >> 20;
		Touch.X = (Touchpad1Raw & 0x000FFF00) >> 8;
		Touch.Down = (Touchpad1Raw & (1 << 7)) == 0;
		Touch.Id = (Touchpad1Raw & 127) % 10;

		bool bIsTouchDown = Touch.Down;
		if (bIsTouchDown)
		{
			if (!IsWasTouch1Down())
			{
				const FVector2D TouchVectorStart = FVector2D(Touch.X, Touch.Y);
				InMessageHandler->OnTouchStarted(nullptr, TouchVectorStart, 1.0f, Touch.Id, UserId, InputDeviceId);
			}
			else
			{
				const FVector2D TouchVector = FVector2D(Touch.X, Touch.Y);
				InMessageHandler->OnTouchMoved(TouchVector, 1.0f, Touch.Id, UserId, InputDeviceId);
			}
		}
		else if (!bIsTouchDown && IsWasTouch1Down())
		{
			const FVector2D TouchVectorEnded = FVector2D(Touch.X, Touch.Y);
			InMessageHandler->OnTouchEnded(TouchVectorEnded, Touch.Id, UserId, InputDeviceId);
		}
		SetWasTouch1Down(bIsTouchDown);

		FTouchPoint2 Touch2;
		const int32 Touchpad2Raw = *reinterpret_cast<const int32*>(&HIDInput[0x24]);
		Touch2.Y = (Touchpad2Raw & 0xFFF00000) >> 20;
		Touch2.X = (Touchpad2Raw & 0x000FFF00) >> 8;
		Touch2.Down = (Touchpad2Raw & (1 << 7)) == 0;
		Touch2.Id = (Touchpad2Raw & 127) % 10;

		bool bIsTouch2Down = Touch2.Down;
		if (bIsTouch2Down)
		{
			if (!IsWasTouch2Down())
			{
				const FVector2D Touch2VectorStart = FVector2D(Touch2.X, Touch2.Y);
				InMessageHandler->OnTouchStarted(nullptr, Touch2VectorStart, 1.0f, Touch2.Id, UserId, InputDeviceId);
			}
			else
			{
				const FVector2D Touch2Vector = FVector2D(Touch2.X, Touch2.Y);
				InMessageHandler->OnTouchMoved(Touch2Vector, 1.0f, Touch2.Id, UserId, InputDeviceId);
			}
		}
		else if (!bIsTouch2Down && IsWasTouch2Down())
		{
			const FVector2D Touch2VectorEnded = FVector2D(Touch2.X, Touch2.Y);
			InMessageHandler->OnTouchEnded(Touch2VectorEnded, Touch2.Id, UserId, InputDeviceId);
		}
		SetWasTouch2Down(bIsTouch2Down);
	}

	if (IsEnableAccelerometerAndGyroscope())
	{
		FVector GyroDeg;
		FVector AccelG;

		using namespace GamepadCalibrationSensors;
		ProcessMotionData(HIDInput, Context->Calibration, GyroDeg, AccelG);

		float gx = FMath::DegreesToRadians(GyroDeg.Z); // Gyroscope.X * GRAVITY_MS2;
		float gy = FMath::DegreesToRadians(GyroDeg.Y); // Gyroscope.Y * GRAVITY_MS2;
		float gz = FMath::DegreesToRadians(GyroDeg.X); // Gyroscope.Z * GRAVITY_MS2;

		float ax = AccelG.Z;
		float ay = AccelG.Y;
		float az = AccelG.X;

		// if (IsResetGyroscope())
		// {
		// 	MadgwickFilter.Reset();
		// 	SetIsResetGyroscope(false);
		// }

		// Update Madgwick filter (IMU-only)
		MadgwickFilter.UpdateImu(gx, gy, gz, ax, ay, az, Delta);

		// Get quaternion directly to avoid Gimbal Lock
		float qw, qx, qy, qz;
		MadgwickFilter.GetQuaternion(qw, qx, qy, qz);

		// Create Unreal quaternion
		const FQuat SensorQuat(qx, qy, qz, qw);
		const FRotator ControlRotation = SensorQuat.Rotator();

		// Compose Tilt (Pitch, Yaw, Roll) in degrees
		const FVector Tilt = FVector(ControlRotation.Roll,
		                             ControlRotation.Yaw,
		                             ControlRotation.Pitch);

		FVector GravityVector = SensorQuat.GetUpVector();

		InMessageHandler.Get().OnMotionDetected(Tilt, GyroDeg, GravityVector, AccelG, UserId, InputDeviceId);
	}

	SetHasPhoneConnected(HIDInput[0x35] & 0x01);
	SetBatteryLevel(((HIDInput[0x34] & 0x0F) / 10.0) * 100);
}

void FDualSenseLibrary::SetResistance(uint8 StartZones, uint8 Strength, const EControllerHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Resistance(Context, StartZones, Strength, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetGalloping23(uint8 StartPosition, uint8 EndPosition, uint8 FirstFoot, uint8 SecondFoot,
                                       uint8 Frequency, const EControllerHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Galloping23(Context, StartPosition, EndPosition, FirstFoot, SecondFoot, Frequency, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::StopTrigger(const EControllerHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Off(Context, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetGameCube(const EControllerHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	GameCube(Context, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetBow22(uint8 StartZone, uint8 SnapBack, const EControllerHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Bow22(Context, StartZone, SnapBack, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetWeapon25(uint8 StartZone, uint8 Amplitude, uint8 Behavior, uint8 Trigger, const EControllerHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Weapon25(Context, StartZone, Amplitude, Behavior, Trigger, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetMachineGun26(uint8 StartZone, uint8 Behavior, uint8 Amplitude, uint8 Frequency, const EControllerHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	MachineGun26(Context, StartZone, Behavior, Amplitude, Frequency, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetMachine27(uint8 StartZone, uint8 BehaviorFlag, uint8 Force, uint8 Amplitude, uint8 Period, uint8 Frequency, const EControllerHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Machine27(Context, StartZone, BehaviorFlag, Force, Amplitude, Period, Frequency, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetCustomTrigger(const EControllerHand& Hand, const TArray<FString>& HexBytes)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	CustomTrigger(Context, Hand, HexBytes);

	UpdateOutput();
}

void FDualSenseLibrary::SetPlayerLed(ELedPlayerEnum Led, ELedBrightnessEnum Brightness)
{
}

void FDualSenseLibrary::SetMicrophoneLed(ELedMicEnum Led)
{
}

void FDualSenseLibrary::AudioHapticUpdate(TArray<int8> Data)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	if (!Context || !Context->IsConnected)
	{
		return;
	}

	unsigned char* AudioData = &Context->BufferAudio[10];
	AudioData[0] = (AudioVibrationSequence++) & 0xFF;
	AudioData[1] = 0x92;
	AudioData[2] = 0x40;
	FMemory::Memcpy(&AudioData[3], Data.GetData(), 64);
	FPlayStationOutputComposer::SendAudioHapticAdvanced(Context);
}
