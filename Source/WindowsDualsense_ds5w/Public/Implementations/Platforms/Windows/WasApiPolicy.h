#pragma once
#include "Helpers/DualSenseLog.h"
#include <cstdint>
#include <filesystem>
#include <limits>
#include <string>
#include <vector>

// clang-format off
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
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

	int NumChannels;
	int SampleRate;
	bool bInitialized = false;
	bool bHasDeviceId = false;

	DevicePathType DevicePath;
	AudioDeviceType Device{};
	AudioRingBufferType RingBuffer{};
	AudioDeviceIdType DeviceId;

	bool Initialize()
	{
		return InitializeWithDeviceId(nullptr, 48000, 2);
	}

	bool InitializeWithDeviceId(const AudioDeviceIdType* InDeviceId, int InSampleRate = 48000, int InNumChannels = 2)
	{
		if (InDeviceId)
		{
			DeviceId = *InDeviceId;
			SampleRate = InSampleRate;
			NumChannels = InNumChannels;
			bHasDeviceId = true;
		}
		else
		{
			DeviceId.clear();
			SampleRate = InSampleRate;
			NumChannels = InNumChannels;
			bHasDeviceId = false;
		}
		return true;
	}

	void RegisterAudioDevice(const DevicePathType& InDevicePath, const AudioDeviceIdType* InDeviceId = nullptr)
	{
		DevicePath = InDevicePath;
		if (InDeviceId)
		{
			DeviceId = *InDeviceId;
			bHasDeviceId = true;
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
		bHasDeviceId = false;
		Device = AudioDeviceType{};
		
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
	
		WriteToWasapiEndpoint(DeviceId, BufferData, FrameCount, NumChannels, SampleRate);

		// Clear the buffer after sending
		RingBuffer.clear();
		return true;
	}
	
	/**
	 * @brief Sends haptic audio data to a WASAPI render endpoint.
	 *
	 * Opens the specified WASAPI audio endpoint by device ID and writes the float buffer
	 * to it. Returns true if successful, false otherwise.
	 *
	 * @param WasapiDeviceId The WASAPI device ID (from PKEY_Device_FriendlyName or similar)
	 * @param inAudioBuffer Pointer to float data to write
	 * @param inFrameCount Number of audio frames to write
	 * @param inNumChannels Number of audio channels (2 or 4)
	 * @param inSampleRate Sample rate in Hz (typically 48000)
	 * @return true if write succeeded, false on error
	 */
	bool WriteToWasapiEndpoint(const std::string& WasapiDeviceId, const float* inAudioBuffer, int inFrameCount, int inNumChannels, int inSampleRate)
	{
		std::wstring inDeviceId = std::filesystem::path(WasapiDeviceId).wstring();
		
		if (!inAudioBuffer || inFrameCount == 0 || inNumChannels < 2)
		{
			return false;
		}

		// Initialize COM
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		bool bComInitialized = SUCCEEDED(hr) || hr == S_FALSE;
		if (!bComInitialized)
		{
			return false;
		}

		IMMDeviceEnumerator* pEnumerator = nullptr;
		IMMDevice* pDevice = nullptr;
		IAudioClient* pAudioClient = nullptr;
		IAudioRenderClient* pRenderClient = nullptr;
		bool bSuccess = false;

		do
		{
			// Create device enumerator
			hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
			if (FAILED(hr) || !pEnumerator)
			{
				UE_LOG(LogDualSense, Error, TEXT("WriteToWasapiEndpoint: Failed to create device enumerator (0x%08X)"), hr);
				break;
			}

			// Get the specified device
			hr = pEnumerator->GetDevice(inDeviceId.c_str(), &pDevice);
			if (FAILED(hr) || !pDevice)
			{
				UE_LOG(LogDualSense, Error, TEXT("WriteToWasapiEndpoint: Failed to get device (0x%08X)"), hr);
				break;
			}

			// Activate the audio client
			hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&pAudioClient);
			if (FAILED(hr) || !pAudioClient)
			{
				UE_LOG(LogDualSense, Error, TEXT("WriteToWasapiEndpoint: Failed to activate audio client (0x%08X)"), hr);
				break;
			}

			// Set up wave format
			WAVEFORMATEX WaveFormat = {};
			WaveFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
			WaveFormat.nChannels = static_cast<WORD>(NumChannels);
			WaveFormat.nSamplesPerSec = static_cast<DWORD>(SampleRate);
			WaveFormat.nAvgBytesPerSec = static_cast<DWORD>(SampleRate * NumChannels * sizeof(float));
			WaveFormat.nBlockAlign = static_cast<WORD>(NumChannels * sizeof(float));
			WaveFormat.wBitsPerSample = 32;
			WaveFormat.cbSize = 0;

			// Initialize the audio client
			hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, &WaveFormat, nullptr);
			if (FAILED(hr))
			{
				UE_LOG(LogDualSense, Error, TEXT("WriteToWasapiEndpoint: Failed to initialize audio client (0x%08X)"), hr);
				break;
			}

			// Get render client
			hr = pAudioClient->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&pRenderClient));
			if (FAILED(hr) || !pRenderClient)
			{
				UE_LOG(LogDualSense, Error, TEXT("WriteToWasapiEndpoint: Failed to get render client (0x%08X)"), hr);
				break;
			}

			// Get buffer size
			UINT32 BufferFrameCount = 0;
			hr = pAudioClient->GetBufferSize(&BufferFrameCount);
			if (FAILED(hr))
			{
				UE_LOG(LogDualSense, Error, TEXT("WriteToWasapiEndpoint: Failed to get buffer size (0x%08X)"), hr);
				break;
			}

			// Fill initial buffer
			BYTE* pBuffer = nullptr;
			hr = pRenderClient->GetBuffer(BufferFrameCount, &pBuffer);
			if (FAILED(hr) || !pBuffer)
			{
				UE_LOG(LogDualSense, Error, TEXT("WriteToWasapiEndpoint: Failed to get buffer (0x%08X)"), hr);
				break;
			}

			// Copy audio data
			uint32 BytesToCopy = BufferFrameCount * static_cast<uint32>(NumChannels) * sizeof(float);
			FMemory::Memcpy(pBuffer, inAudioBuffer, BytesToCopy);

			// Release buffer
			hr = pRenderClient->ReleaseBuffer(BufferFrameCount, 0);
			if (FAILED(hr))
			{
				UE_LOG(LogDualSense, Error, TEXT("WriteToWasapiEndpoint: Failed to release buffer (0x%08X)"), hr);
				break;
			}

			// Start playback
			hr = pAudioClient->Start();
			if (FAILED(hr))
			{
				UE_LOG(LogDualSense, Warning, TEXT("WriteToWasapiEndpoint: Audio client already started or failed to start (0x%08X)"), hr);
				// This might not be a fatal error if already running
			}

			bSuccess = true;

		} while (false);

		// Cleanup
		if (pRenderClient)
		{
			pRenderClient->Release();
		}
		if (pAudioClient)
		{
			pAudioClient->Stop();
			pAudioClient->Release();
		}
		if (pDevice)
		{
			pDevice->Release();
		}
		if (pEnumerator)
		{
			pEnumerator->Release();
		}

		if (bComInitialized)
		{
			CoUninitialize();
		}

		return bSuccess;
	}
};
