#pragma once
#include "Helpers/DualSenseLog.h"
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

// clang-format off
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif
// clang-format on


struct FAudioDeviceInfo
{
	std::string Id;           // O ID maluco que o WASAPI precisa: "{0.0.0.0000}..."
	std::string FriendlyName; // O nome legível: "Speakers (Wireless Controller)"
};

struct WasApiPolicy
{
public:
	using DevicePathType = std::string;
	using AudioDeviceType = FAudioDeviceInfo;
	using AudioDeviceIdType = std::string;

	// In this policy we keep a software staging buffer. The actual WASAPI write happens elsewhere.
	using AudioRingBufferType = std::vector<float>;
	using AudioFrameCountType = int;

	int NumChannels = 2;
	int SampleRate = 48000;
	bool bInitialized = false;
	bool bHasDeviceId = false;
	bool bComInitialized = false;
	bool bAudioStarted = false;

	DevicePathType DevicePath;
	AudioDeviceType Device{};
	AudioRingBufferType RingBuffer{};
	AudioDeviceIdType DeviceId;

#if PLATFORM_WINDOWS
	IMMDeviceEnumerator* DeviceEnumerator = nullptr;
	IMMDevice* DeviceEndpoint = nullptr;
	IAudioClient* AudioClient = nullptr;
	IAudioRenderClient* AudioRenderClient = nullptr;
	UINT32 WasapiBufferFrameCount = 0;
#endif

	~WasApiPolicy()
	{
		Close();
	}

	bool Initialize()
	{
		return InitializeWithDeviceId(nullptr, 48000, 2);
	}

	bool InitializeWithDeviceId(const AudioDeviceIdType* InDeviceId, int InSampleRate = 48000, int InNumChannels = 2)
	{
		Close();

		SampleRate = InSampleRate;
		NumChannels = InNumChannels;

		if (InDeviceId)
		{
			DeviceId = *InDeviceId;
			bHasDeviceId = true;
		}
		else
		{
			DeviceId.clear();
			bHasDeviceId = false;
		}

		if (!bHasDeviceId)
		{
			return false;
		}

		bInitialized = InitializeWasapiClient();
		return bInitialized;
	}

	void RegisterAudioDevice(const DevicePathType& InDevicePath, const AudioDeviceIdType* InDeviceId = nullptr)
	{
		DevicePath = InDevicePath;
		if (InDeviceId)
		{
			InitializeWithDeviceId(InDeviceId, SampleRate, NumChannels);
		}
	}

	void UnregisterAudioDevice(const DevicePathType& InDevicePath)
	{
		if (DevicePath == InDevicePath)
		{
			Close();
		}
	}

	void Close()
	{
		bInitialized = false;

#if PLATFORM_WINDOWS
		if (AudioClient && bAudioStarted)
		{
			AudioClient->Stop();
			bAudioStarted = false;
		}

		if (AudioRenderClient)
		{
			AudioRenderClient->Release();
			AudioRenderClient = nullptr;
		}

		if (AudioClient)
		{
			AudioClient->Release();
			AudioClient = nullptr;
		}

		if (DeviceEndpoint)
		{
			DeviceEndpoint->Release();
			DeviceEndpoint = nullptr;
		}

		if (DeviceEnumerator)
		{
			DeviceEnumerator->Release();
			DeviceEnumerator = nullptr;
		}

		WasapiBufferFrameCount = 0;

		if (bComInitialized)
		{
			CoUninitialize();
			bComInitialized = false;
		}
#endif

		bInitialized = false;
		bHasDeviceId = false;
		Device = AudioDeviceType{};
		bAudioStarted = false;

		DeviceId.clear();
		DevicePath.clear();
		RingBuffer.clear();
	}

	[[nodiscard]] bool IsValid() const
	{
		return bInitialized;
	}

	static AudioFrameCountType GetAvailableWriteFrames()
	{
		return std::numeric_limits<AudioFrameCountType>::max();
	}

	/**
	 * @brief Converts interleaved audio data to a normalized float buffer and sends it to a WASAPI endpoint.
	 *
	 * This function processes interleaved audio input data, converts it to a normalized float format,
	 * and writes it to an internal buffer. It then sends the buffer to a WASAPI render endpoint. This
	 * function supports stereo and quad-channel configurations, with proper normalization and mapping
	 * of channels, and clears the internal buffer after transmission.
	 *
	 * @param InterleavedData A vector of int16_t interleaved audio samples, where every two samples represent
	 *        left and right channel data in stereo.
	 * @return true if haptic data is successfully written to the WASAPI endpoint, false otherwise.
	 */
	bool WriteHapticData(const std::vector<std::int16_t>& InterleavedData)
	{
		if (!IsValid() || InterleavedData.empty() || NumChannels < 2)
		{
			return false;
		}

		const AudioFrameCountType FramesToWrite = static_cast<AudioFrameCountType>(InterleavedData.size() / 2);
		if (FramesToWrite == 0)
		{
			return true;
		}

		RingBuffer.resize(static_cast<size_t>(FramesToWrite) * static_cast<size_t>(NumChannels));

		constexpr float kNormalization = 1.0f / 32768.0f;
		for (AudioFrameCountType i = 0; i < FramesToWrite; ++i)
		{
			const float LeftFloat = static_cast<float>(InterleavedData[static_cast<size_t>(i) * 2]) * kNormalization;
			const float RightFloat = static_cast<float>(InterleavedData[(static_cast<size_t>(i) * 2) + 1]) * kNormalization;
			const size_t BaseIndex = static_cast<size_t>(i) * static_cast<size_t>(NumChannels);

			if (NumChannels >= 4)
			{
				RingBuffer[BaseIndex + 0] = 0.f;
				RingBuffer[BaseIndex + 1] = 0.f;
				RingBuffer[BaseIndex + 2] = LeftFloat;
				RingBuffer[BaseIndex + 3] = RightFloat;
			}
			else
			{
				RingBuffer[BaseIndex + 0] = LeftFloat;
				RingBuffer[BaseIndex + 1] = RightFloat;
			}
		}
		
		// Send the pending buffer to the WASAPI endpoint
		const float* BufferData = RingBuffer.data();
		int FrameCount = RingBuffer.size() / NumChannels;
		const bool bWriteOk = WriteToWasapiEndpoint(BufferData, FrameCount, NumChannels, SampleRate);

		// Clear the buffer after sending
		RingBuffer.clear();
		return bWriteOk;
	}
	
	/**
	 * @brief Sends haptic audio data to a WASAPI render endpoint.
	 *
	 * Opens the specified WASAPI audio endpoint by device ID and writes the float buffer
	 * to it. Returns true if successful, false otherwise.
	 *
	 * @param inAudioBuffer Pointer to float data to write
	 * @param inFrameCount Number of audio frames to write
	 * @param inNumChannels Number of audio channels (2 or 4)
	 * @param inSampleRate Sample rate in Hz (typically 48000)
	 * @return true if write succeeded, false on error
	 */
	bool WriteToWasapiEndpoint(const float* inAudioBuffer, int inFrameCount, int inNumChannels, int inSampleRate)
	{
		if (!inAudioBuffer || inFrameCount <= 0 || inNumChannels < 2)
		{
			return false;
		}

		if (!AudioClient || !AudioRenderClient || !bHasDeviceId || DeviceId.empty())
		{
			return false;
		}

		if (inNumChannels != NumChannels || inSampleRate != SampleRate)
		{
			return false;
		}

		UINT32 CurrentPadding = 0;
		HRESULT hr = AudioClient->GetCurrentPadding(&CurrentPadding);
		if (FAILED(hr))
		{
			UE_LOG(LogDualSense, Warning, TEXT("WriteToWasapiEndpoint: GetCurrentPadding failed (0x%08X)"), hr);
			return false;
		}

		const UINT32 FramesAvailable = WasapiBufferFrameCount > CurrentPadding ? (WasapiBufferFrameCount - CurrentPadding) : 0;
		const UINT32 FramesToWrite = static_cast<UINT32>(FramesAvailable < static_cast<UINT32>(inFrameCount) ? FramesAvailable : static_cast<UINT32>(inFrameCount));
		if (FramesToWrite == 0)
		{
			return true;
		}

		BYTE* OutBuffer = nullptr;
		hr = AudioRenderClient->GetBuffer(FramesToWrite, &OutBuffer);
		if (FAILED(hr) || !OutBuffer)
		{
			UE_LOG(LogDualSense, Warning, TEXT("WriteToWasapiEndpoint: GetBuffer failed (0x%08X)"), hr);
			return false;
		}

		const SIZE_T BytesToCopy = static_cast<SIZE_T>(FramesToWrite) * static_cast<SIZE_T>(inNumChannels) * sizeof(float);
		FMemory::Memcpy(OutBuffer, inAudioBuffer, BytesToCopy);

		hr = AudioRenderClient->ReleaseBuffer(FramesToWrite, 0);
		if (FAILED(hr))
		{
			UE_LOG(LogDualSense, Warning, TEXT("WriteToWasapiEndpoint: ReleaseBuffer failed (0x%08X)"), hr);
			return false;
		}

		return true;
	}

private:
	bool ConvertToWide(const std::string& InValue, std::wstring& OutValue) const
	{
		if (InValue.empty())
		{
			return false;
		}

		int WideCount = MultiByteToWideChar(CP_UTF8, 0, InValue.c_str(), -1, nullptr, 0);
		UINT CodePage = CP_UTF8;
		if (WideCount <= 0)
		{
			CodePage = CP_ACP;
			WideCount = MultiByteToWideChar(CodePage, 0, InValue.c_str(), -1, nullptr, 0);
		}
		if (WideCount <= 0)
		{
			return false;
		}

		OutValue.resize(static_cast<size_t>(WideCount));
		if (MultiByteToWideChar(CodePage, 0, InValue.c_str(), -1, OutValue.data(), WideCount) <= 0)
		{
			return false;
		}

		if (!OutValue.empty() && OutValue.back() == L'\0')
		{
			OutValue.pop_back();
		}

		return true;
	}

	bool InitializeWasapiClient()
	{
#if !PLATFORM_WINDOWS
		return false;
#else
		if (DeviceId.empty())
		{
			return false;
		}

		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
		{
			UE_LOG(LogDualSense, Error, TEXT("InitializeWasapiClient: CoInitializeEx failed (0x%08X)"), hr);
			return false;
		}
		bComInitialized = SUCCEEDED(hr);

		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&DeviceEnumerator));
		if (FAILED(hr) || !DeviceEnumerator)
		{
			UE_LOG(LogDualSense, Error, TEXT("InitializeWasapiClient: CoCreateInstance failed (0x%08X)"), hr);
			Close();
			return false;
		}

		std::wstring WideDeviceId;
		if (!ConvertToWide(DeviceId, WideDeviceId))
		{
			UE_LOG(LogDualSense, Error, TEXT("InitializeWasapiClient: Failed converting device id to wide string."));
			Close();
			return false;
		}

		hr = DeviceEnumerator->GetDevice(WideDeviceId.c_str(), &DeviceEndpoint);
		if (FAILED(hr) || !DeviceEndpoint)
		{
			UE_LOG(LogDualSense, Error, TEXT("InitializeWasapiClient: GetDevice failed (0x%08X)"), hr);
			Close();
			return false;
		}

		hr = DeviceEndpoint->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&AudioClient));
		if (FAILED(hr) || !AudioClient)
		{
			UE_LOG(LogDualSense, Error, TEXT("InitializeWasapiClient: Activate(IAudioClient) failed (0x%08X)"), hr);
			Close();
			return false;
		}

		const WAVEFORMATEX WaveFormat = {
			WAVE_FORMAT_IEEE_FLOAT,
			static_cast<WORD>(NumChannels),
			static_cast<DWORD>(SampleRate),
			static_cast<DWORD>(SampleRate * NumChannels * sizeof(float)),
			static_cast<WORD>(NumChannels * sizeof(float)),
			32,
			0};

		hr = AudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, &WaveFormat, nullptr);
		if (FAILED(hr))
		{
			UE_LOG(LogDualSense, Error, TEXT("InitializeWasapiClient: AudioClient->Initialize failed (0x%08X)"), hr);
			Close();
			return false;
		}

		hr = AudioClient->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&AudioRenderClient));
		if (FAILED(hr) || !AudioRenderClient)
		{
			UE_LOG(LogDualSense, Error, TEXT("InitializeWasapiClient: GetService(IAudioRenderClient) failed (0x%08X)"), hr);
			Close();
			return false;
		}

		hr = AudioClient->GetBufferSize(&WasapiBufferFrameCount);
		if (FAILED(hr) || WasapiBufferFrameCount == 0)
		{
			UE_LOG(LogDualSense, Error, TEXT("InitializeWasapiClient: GetBufferSize failed (0x%08X)"), hr);
			Close();
			return false;
		}

		hr = AudioClient->Start();
		if (FAILED(hr))
		{
			UE_LOG(LogDualSense, Error, TEXT("InitializeWasapiClient: AudioClient->Start failed (0x%08X)"), hr);
			Close();
			return false;
		}

		bAudioStarted = true;
		return true;
#endif
	}
};
