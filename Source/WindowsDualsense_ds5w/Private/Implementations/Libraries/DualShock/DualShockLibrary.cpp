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
	FInputContext* InputToFill = Context->GetBackBuffer();

	using namespace FGamepadProcessInput;
	if (Context->ConnectionType == EDeviceConnection::Bluetooth)
	{
		DualShockRaw(&Context->BufferDS4[3], InputToFill);
	}
	else
	{
		DualShockRaw(&Context->Buffer[1], InputToFill);
	}
}

void FDualShockLibrary::SetVibration(uint8 LeftRumble, uint8 RightRumble)
{
	FDeviceContext* Context = GetMutableDeviceContext();
	if (!Context)
	{
		return;
	}

	FOutputContext* HidOutput = &Context->Output;
	if (HidOutput->Rumbles.Left != LeftRumble || HidOutput->Rumbles.Right != RightRumble)
	{
		HidOutput->Rumbles = {LeftRumble, RightRumble};
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
