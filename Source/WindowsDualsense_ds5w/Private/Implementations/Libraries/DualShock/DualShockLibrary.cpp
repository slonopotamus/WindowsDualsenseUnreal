// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Implementations/Libraries/DualShock/DualShockLibrary.h"
#include "Core/Interfaces/IPlatformHardwareInfo.h"
#include "Core/Types/Enums/EDeviceConnection.h"
#include "Helpers/ValidateHelpers.h"
#include "Implementations/Utils/GamepadProcessInput.h"
#include "Implementations/Utils/PlayStationOutputComposer.h"

bool FDualShockLibrary::Initialize(const FDeviceContext& Context)
{
	SetDeviceContexts(Context);
	SetLightbar(FColor::Blue, 0.0f, 0.0f);
	return true;
}

void FDualShockLibrary::UpdateOutput()
{
	FDeviceContext* Context = GetMutableDeviceContext();
	if (!Context->IsConnected)
	{
		return;
	}

	FPlayStationOutputComposer::OutputDualShock(Context);
}

void FDualShockLibrary::UpdateInput(float Delta)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	IPlatformHardwareInfo::Get().Read(Context);

	using namespace GamepadProcessInput;
	if (Context->ConnectionType == EDeviceConnection::Bluetooth)
	{
		DualShockRaw(&Context->Buffer[3], &Context->Input);
	}
	else
	{
		DualShockRaw(&Context->Buffer[1], &Context->Input);
	}
}

void FDualShockLibrary::SetVibration(const FForceFeedbackValues& Values)
{
	FDeviceContext* Context = GetMutableDeviceContext();
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

void FDualShockLibrary::SetLightbar(FColor Color, float BrithnessTime, float ToggleTime)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	FOutputContext* HidOutput = &Context->Output;
	HidOutput->Lightbar.R = Color.R;
	HidOutput->Lightbar.G = Color.G;
	HidOutput->Lightbar.B = Color.B;

	HidOutput->FlashLigthbar.Bright_Time = static_cast<unsigned char>(FValidateHelpers::To255(BrithnessTime));
	HidOutput->FlashLigthbar.Toggle_Time = static_cast<unsigned char>(FValidateHelpers::To255(ToggleTime));
	UpdateOutput();
}

void FDualShockLibrary::ResetLights()
{
	SetLightbar(FColor::Blue, 0.0f, 0.0f);
}
