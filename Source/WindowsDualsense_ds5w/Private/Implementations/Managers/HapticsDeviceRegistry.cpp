// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Implementations/Managers/HapticsDeviceRegistry.h"
#include "API/SonyGamepadProxyHelpers.h"
#include "Async/Async.h"
#include "AudioDevice.h"
#include "Sound/SoundSubmix.h"

#include "Misc/App.h"
#include "Runtime/Launch/Resources/Version.h"
#include <functional>

TSharedPtr<FHapticsDeviceRegistry> FHapticsDeviceRegistry::Instance;

FHapticsDeviceRegistry::~FHapticsDeviceRegistry()
{
	RemoveAllListeners();
	if (Instance->GameThreadTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(Instance->GameThreadTickerHandle);
	}
}

bool FHapticsDeviceRegistry::HasListenerForDevice(int32 DeviceId) const
{
	return ControllerListeners.Contains(DeviceId);
}

TSharedPtr<FHapticsDeviceRegistry> FHapticsDeviceRegistry::Get()
{
	if (!Instance)
	{
		check(IsInGameThread());
		Instance = MakeShared<FHapticsDeviceRegistry>();

		Instance->GameThreadTickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		    FTickerDelegate::CreateSP(Instance.Get(), &FHapticsDeviceRegistry::Tick));
	}
	return Instance;
}

void FHapticsDeviceRegistry::CreateListenerForDevice(int32 DeviceId, USoundSubmix* Submix)
{
	if (!Submix)
	{
		return;
	}

	using namespace SonyGamepadProxyHelpers;
	auto* Gamepad = GetGamepad(DeviceId);
	if (!Gamepad)
	{
		return;
	}

	if (Gamepad->GetConnectionType() == EDSDeviceConnection::Usb)
	{
		if (Gamepad->GetIGamepadHaptics())
		{
			FDeviceContext* Ctx = Gamepad->GetMutableDeviceContext();
			if (!Ctx)
			{
				return;
			}
			InitializeAudioContainer(Ctx);
		}
	}

	if (ControllerListeners.Contains(DeviceId))
	{
		UE_LOG(LogDualSense, Warning, TEXT("Controller %d already has a listener registered."), DeviceId);
		return;
	}

	bool IsWireless = Gamepad->GetConnectionType() == EDSDeviceConnection::Bluetooth;
	const TSharedPtr<FAudioHapticsListener> Listener = MakeShared<FAudioHapticsListener>(DeviceId, Submix, IsWireless);
	if (FAudioDeviceHandle AudioDevice = GEngine->GetActiveAudioDevice())
	{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 3
		AudioDevice->RegisterSubmixBufferListener(Listener.ToSharedRef(), *Submix);
#else
		AudioDevice->RegisterSubmixBufferListener(Listener.Get(), Submix);
#endif

		UE_LOG(LogDualSense, Log, TEXT("Controller %d Listener %s"), DeviceId, *Submix->GetName());
		ControllerListeners.Add(DeviceId, Listener);
	}
}

void FHapticsDeviceRegistry::RemoveAllListeners()
{
	if (FAudioDeviceHandle AudioDevice = GEngine->GetActiveAudioDevice())
	{
		for (auto& Pair : ControllerListeners)
		{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 3
			AudioDevice->UnregisterSubmixBufferListener(Pair.Value.ToSharedRef(), *Pair.Value.ToSharedRef()->GetSubmix());
#else
			AudioDevice->UnregisterSubmixBufferListener(Pair.Value.Get());
#endif
		}
	}
	ControllerListeners.Empty();
}

bool FHapticsDeviceRegistry::Tick(float DeltaTime)
{
	for (auto& Pair : ControllerListeners)
	{
		if (Pair.Value.IsValid())
		{
			TSharedPtr<FAudioHapticsListener> Context = Pair.Value;
			if (IGamepadHaptics* HapticsInterface = SonyGamepadProxyHelpers::GetAudioHapticsInterface(Pair.Key))
			{
				AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [NewContext = MoveTemp(Context), NewHapticsInterface = MoveTemp(HapticsInterface)]() {
					NewContext->ConsumeHapticsQueue(NewHapticsInterface);
				});
			}
		}
	}
	return true;
}

void FHapticsDeviceRegistry::RemoveListenerForDevice(int32 DeviceId)
{
	if (const TSharedPtr<FAudioHapticsListener>* ExistingListener = ControllerListeners.Find(DeviceId))
	{
		if (FAudioDeviceHandle AudioDevice = GEngine->GetActiveAudioDevice())
		{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 3
			AudioDevice->UnregisterSubmixBufferListener(ExistingListener->ToSharedRef(), *ExistingListener->ToSharedRef()->GetSubmix());
#else
			AudioDevice->UnregisterSubmixBufferListener(ExistingListener->Get());
#endif
		}
		ControllerListeners.Remove(DeviceId);
	}
}

void FHapticsDeviceRegistry::InitializeAudioContainer(FDeviceContext* Context)
{
	if (!Context)
	{
		UE_LOG(LogDualSense, Error, TEXT("InitializeAudioContainer: Context is null"));
		return;
	}

	if (const auto it = DevicePolicies.find(Context->Path); it != DevicePolicies.end())
	{
		UE_LOG(LogDualSense, Error, TEXT("InitializeAudioContainer: Device policy already exists"));
		return;
	}
	
	std::string AudioInfo = FWindowsDeviceInfo::InitializeAudioDevice(Context);
	if (AudioInfo.empty())
	{
		UE_LOG(LogDualSense, Warning, TEXT("InitializeAudioContainer: No audio endpoint matched for this device."));
		return;
	}

	FAudioDeviceInfo AudioDeviceInfo;
	AudioDeviceInfo.Id = AudioInfo;
	
	auto Policy = std::make_shared<GamepadCore::TAudioDeviceRegistry<AudioHapticsHardwarePolicy>>();
	Policy->Policy.Device = AudioDeviceInfo;
	Policy->Policy.DevicePath = Context->Path;
	if (!Policy->Policy.InitializeWithDeviceId(&AudioDeviceInfo.Id, 48000, 2))
	{
		UE_LOG(LogDualSense, Error, TEXT("InitializeAudioContainer: Failed to initialize WASAPI policy for %s."), *FString(AudioInfo.c_str()));
		return;
	}
	DevicePolicies.emplace(Context->Path, Policy);

	FString DeviceName(AudioInfo.c_str());
	UE_LOG(LogDualSense, Log, TEXT("InitializeAudioContainer: Registered audio policy for %s."), *DeviceName);
}

void FHapticsDeviceRegistry::ProcessAudioHaptic(FDeviceContext* Context, const std::vector<std::int16_t>& AudioData)
{
	if (!Context || AudioData.empty())
	{
		return;
	}

	// Only process USB connections for audio haptics
	if (Context->ConnectionType != EDSDeviceConnection::Usb)
	{
		return;
	}

	// Find the policy mapped to this device path
	const auto it = DevicePolicies.find(Context->Path);
	if (it == DevicePolicies.end() || !it->second)
	{
		return;
	}

	auto& Policy = it->second;
	if (!Policy->IsValid())
	{
		return;
	}

	Policy->WriteHapticData(AudioData);
}
