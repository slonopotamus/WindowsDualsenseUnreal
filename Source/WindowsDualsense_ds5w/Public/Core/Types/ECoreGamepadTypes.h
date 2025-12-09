#pragma once

#include "Core/Types/DSCoreTypes.h"

using namespace DSCoreTypes;

/**
 * @brief Enum class representing various LED microphone states.
 */
enum class EDSMic : DSCoreTypes::uint8
{
	MicOn = 0x0,
	MicOff = 0x1,
	Pulse = 0x2
};

/**
 * @brief Player LED definition logic.
 */
enum class EDSPlayer : DSCoreTypes::uint8
{
	Off = 0,
	One = DSCoreTypes::LedMasks::Middle,
	Two = DSCoreTypes::LedMasks::MiddleRight | DSCoreTypes::LedMasks::MiddleLeft,
	Three = DSCoreTypes::LedMasks::Right | DSCoreTypes::LedMasks::Middle | DSCoreTypes::LedMasks::Left,
	All = DSCoreTypes::LedMasks::Right | DSCoreTypes::LedMasks::MiddleRight | DSCoreTypes::LedMasks::MiddleLeft | DSCoreTypes::LedMasks::Left
};

/**
 * @brief Audio feature states.
 */
enum class EDSAudioMode : DSCoreTypes::uint8
{
	On,
	Off
};

enum class EDSVibrationMode : DSCoreTypes::uint8
{
	Off = 0,
	DefaultRumble = 0xFF,
	HapticsRumble = 0xFC
};

enum class EDSGamepadHand : DSCoreTypes::uint8
{
	Left,
	Right,
	AnyHand
};

enum class EDSTouchInteraction : uint8
{
	None,
	Swipe,
	Zoom,
	Scroll
};

enum class EDSDeviceType : DSCoreTypes::uint8
{
	DualSense,
	DualSenseEdge,
	DualShock4,
	NotFound
};

enum class EDSDeviceConnection : DSCoreTypes::uint8
{
	Usb,
	Bluetooth,
	Unrecognized
};

class ECoreGamepadTypes
{
};
