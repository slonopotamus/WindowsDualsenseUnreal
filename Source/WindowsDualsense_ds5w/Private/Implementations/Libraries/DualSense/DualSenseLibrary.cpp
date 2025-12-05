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
#include "Implementations/Utils/GamepadProcessInput.h"
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
void FDualSenseLibrary::UpdateInput(float Delta)
{
	// if (IsEnableTouch())
	// {
	// 	FTouchPoint1 Touch;
	// 	const int32 Touchpad1Raw = *reinterpret_cast<const int32*>(&HIDInput[0x20]);
	// 	Touch.Y = (Touchpad1Raw & 0xFFF00000) >> 20;
	// 	Touch.X = (Touchpad1Raw & 0x000FFF00) >> 8;
	// 	Touch.Down = (Touchpad1Raw & (1 << 7)) == 0;
	// 	Touch.Id = (Touchpad1Raw & 127) % 10;
	//
	// 	bool bIsTouchDown = Touch.Down;
	// 	if (bIsTouchDown)
	// 	{
	// 		if (!IsWasTouch1Down())
	// 		{
	// 			const FVector2D TouchVectorStart = FVector2D(Touch.X, Touch.Y);
	// 			InMessageHandler->OnTouchStarted(nullptr, TouchVectorStart, 1.0f, Touch.Id, UserId, InputDeviceId);
	// 		}
	// 		else
	// 		{
	// 			const FVector2D TouchVector = FVector2D(Touch.X, Touch.Y);
	// 			InMessageHandler->OnTouchMoved(TouchVector, 1.0f, Touch.Id, UserId, InputDeviceId);
	// 		}
	// 	}
	// 	else if (!bIsTouchDown && IsWasTouch1Down())
	// 	{
	// 		const FVector2D TouchVectorEnded = FVector2D(Touch.X, Touch.Y);
	// 		InMessageHandler->OnTouchEnded(TouchVectorEnded, Touch.Id, UserId, InputDeviceId);
	// 	}
	// 	SetWasTouch1Down(bIsTouchDown);
	//
	// 	FTouchPoint2 Touch2;
	// 	const int32 Touchpad2Raw = *reinterpret_cast<const int32*>(&HIDInput[0x24]);
	// 	Touch2.Y = (Touchpad2Raw & 0xFFF00000) >> 20;
	// 	Touch2.X = (Touchpad2Raw & 0x000FFF00) >> 8;
	// 	Touch2.Down = (Touchpad2Raw & (1 << 7)) == 0;
	// 	Touch2.Id = (Touchpad2Raw & 127) % 10;
	//
	// 	bool bIsTouch2Down = Touch2.Down;
	// 	if (bIsTouch2Down)
	// 	{
	// 		if (!IsWasTouch2Down())
	// 		{
	// 			const FVector2D Touch2VectorStart = FVector2D(Touch2.X, Touch2.Y);
	// 			InMessageHandler->OnTouchStarted(nullptr, Touch2VectorStart, 1.0f, Touch2.Id, UserId, InputDeviceId);
	// 		}
	// 		else
	// 		{
	// 			const FVector2D Touch2Vector = FVector2D(Touch2.X, Touch2.Y);
	// 			InMessageHandler->OnTouchMoved(Touch2Vector, 1.0f, Touch2.Id, UserId, InputDeviceId);
	// 		}
	// 	}
	// 	else if (!bIsTouch2Down && IsWasTouch2Down())
	// 	{
	// 		const FVector2D Touch2VectorEnded = FVector2D(Touch2.X, Touch2.Y);
	// 		InMessageHandler->OnTouchEnded(Touch2VectorEnded, Touch2.Id, UserId, InputDeviceId);
	// 	}
	// 	SetWasTouch2Down(bIsTouch2Down);
	// }
	
	FDeviceContext* Context = GetMutableDeviceContext();
	if (!Context || !Context->IsConnected)
	{
		return;
	}
	
	IPlatformHardwareInfo::Get().Read(Context);
	FInputContext* InputToFill = Context->GetBackBuffer();
	const size_t Padding = Context->ConnectionType == EDeviceConnection::Bluetooth ? 2 : 1;
	
	using namespace GamepadProcessInput;
	DualSenseRaw(&Context->Buffer[Padding], InputToFill);
	
	if (IsEnableAccelerometerAndGyroscope())
	{
		FVector GyroDeg;
		FVector AccelG;
		using namespace GamepadCalibrationSensors;
		ProcessMotionData(&Context->Buffer[Padding], Context->Calibration, GyroDeg, AccelG);

		constexpr float GToMSq = GRAVITY_MS2;
		constexpr float DegToRad = PI / 180.0f;
		FVector GyroRad = GyroDeg * DegToRad; // deg/s -> rad/s
		FVector AccelRad = AccelG * GToMSq;
		MadgwickFilter.UpdateImu(GyroRad.Z, GyroRad.Y, -GyroRad.X, AccelRad.Z, AccelRad.Y, -AccelRad.X, 0.033f);

		float qw, qx, qy, qz;
		MadgwickFilter.GetQuaternion(qw, qx, qy, qz);

		const FQuat RawQuat(qx, qy, qz, qw);
		const FQuat CorrectionQuat(FVector::ForwardVector, PI);
		const FQuat SensorQuat = CorrectionQuat * RawQuat;

		const FRotator ControlRotation = SensorQuat.Rotator();
		InputToFill->Gyroscope = GyroDeg;
		InputToFill->Accelerometer = AccelG;
		
		InputToFill->Gravity = SensorQuat.GetUpVector();
		InputToFill->Tilt = FVector(ControlRotation.Roll, ControlRotation.Yaw, ControlRotation.Pitch);
	}

	Context->SwapInputBuffers();
}

void FDualSenseLibrary::UpdateOutput()
{
	FDeviceContext* Context = GetMutableDeviceContext();
	if (!Context || !Context->IsConnected)
	{
		return;
	}
	FPlayStationOutputComposer::OutputDualSense(Context);
}

// void FDualSenseLibrary::Settings(const FDualSenseFeatureReport& Settings)
// {
// 	FDeviceContext* Context = GetMutableDeviceContext();
// 	if (Settings.VibrationMode == EDualSenseDeviceFeatureReport::Off)
// 	{
// 		Context->Output.Feature.VibrationMode = 0xFC;
// 	}
//
// 	Context->Output.Feature.SoftRumbleReduce = static_cast<uint8>(Settings.SoftRumbleReduce);
// 	Context->Output.Feature.TriggerSoftnessLevel = static_cast<uint8>(Settings.TriggerSoftnessLevel);
// 	Context->Output.Audio.MicStatus = static_cast<uint8>(Settings.MicStatus);
// 	Context->Output.Audio.MicVolume = static_cast<uint8>(Settings.MicVolume);
// 	Context->Output.Audio.HeadsetVolume = static_cast<uint8>(Settings.AudioVolume);
// 	Context->Output.Audio.SpeakerVolume = static_cast<uint8>(Settings.AudioVolume);
// 	Context->Output.Audio.Mode = 0x08;
// 	if (Settings.AudioHeadset == EDualSenseAudioFeatureReport::On && Settings.AudioSpeaker == EDualSenseAudioFeatureReport::Off)
// 	{
// 		Context->Output.Audio.Mode = 0x31;
// 	}
//
// 	if (Settings.AudioHeadset == EDualSenseAudioFeatureReport::Off && Settings.AudioSpeaker == EDualSenseAudioFeatureReport::On)
// 	{
// 		Context->Output.Audio.Mode = 0x21;
// 	}
// 	UpdateOutput();
// }

void FDualSenseLibrary::SetResistance(uint8 StartZones, uint8 Strength, const EGamepadHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Resistance(Context, StartZones, Strength, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetGalloping23(uint8 StartPosition, uint8 EndPosition, uint8 FirstFoot, uint8 SecondFoot,
                                       uint8 Frequency, const EGamepadHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Galloping23(Context, StartPosition, EndPosition, FirstFoot, SecondFoot, Frequency, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::StopTrigger(const EGamepadHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Off(Context, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetGameCube(const EGamepadHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	GameCube(Context, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetBow22(uint8 StartZone, uint8 SnapBack, const EGamepadHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Context->bOverrideTriggerBytes = false;
	Bow22(Context, StartZone, SnapBack, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetWeapon25(uint8 StartZone, uint8 Amplitude, uint8 Behavior, uint8 Trigger, const EGamepadHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Weapon25(Context, StartZone, Amplitude, Behavior, Trigger, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetMachineGun26(uint8 StartZone, uint8 Behavior, uint8 Amplitude, uint8 Frequency, const EGamepadHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	MachineGun26(Context, StartZone, Behavior, Amplitude, Frequency, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetMachine27(uint8 StartZone, uint8 BehaviorFlag, uint8 Force, uint8 Amplitude, uint8 Period, uint8 Frequency, const EGamepadHand& Hand)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	Machine27(Context, StartZone, BehaviorFlag, Force, Amplitude, Period, Frequency, Hand);
	UpdateOutput();
}

void FDualSenseLibrary::SetCustomTrigger(const EGamepadHand& Hand, const TArray<FString>& HexBytes)
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
