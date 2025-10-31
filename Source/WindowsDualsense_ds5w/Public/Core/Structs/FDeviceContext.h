// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include <vector>


#if PLATFORM_WINDOWS

#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
using FPlatformDeviceHandle = HANDLE;
#define INVALID_PLATFORM_HANDLE INVALID_HANDLE_VALUE

#elif PLATFORM_MAC || PLATFORM_LINUX
#include "SDL_hidapi.h"
using FPlatformDeviceHandle = SDL_hid_device*;
#define INVALID_PLATFORM_HANDLE nullptr
#else
using FPlatformDeviceHandle = void*;
#define INVALID_PLATFORM_HANDLE nullptr
#endif

#include "CoreMinimal.h"
#include "FOutputContext.h"
#include "Core/Enums/EDeviceConnection.h"
#include "FDeviceContext.generated.h"

/**
 * @brief Represents a haptic feedback data structure for DualSense devices.
 *
 * This structure is designed to form and manage the data packets required
 * for sending advanced haptic feedback commands to DualSense controllers.
 * It includes substructures that define the format and content of the
 * necessary packet headers and payloads for haptic vibration transmission.
 *
 * The primary purpose is to enable precise and efficient configuration
 * of haptic features such as audio-based vibration feedback by organizing
 * data into specific packets (e.g., 0x11 and 0x12) for internal controller
 * processing.
 *
 * Features:
 * - Contains a union for raw and formatted data representation.
 * - Automatically initializes packets with default values for ease of use.
 * - Aligns properly for hardware-level communication to ensure correct
 *   data transfers to the device.
 */
USTRUCT()
struct FDualSenseHapticBuffer {
	GENERATED_BODY()
	// /**
	//  * @brief Represents the header structure for a DualSense haptic feedback packet.
	//  *
	//  * This structure defines the format of the packet header used in constructing
	//  * haptic feedback data for DualSense devices. It includes essential fields
	//  * necessary for identifying and sequencing individual packets sent to the
	//  * controller.
	//  *
	//  * Features:
	//  * - The Report_ID field uniquely identifies the type of the report being sent.
	//  * - The Tag_Seq field is used to manage sequence control and tag information
	//  *   across transmitted packets.
	//  *
	//  * This structure is specifically designed to align with the communication
	//  * protocol requirements of DualSense devices, ensuring accurate and efficient
	//  * data transmission.
	//  */
	// struct FPacketHeader {
	// 	uint8 Report_ID;
	// 	uint8 Tag_Seq;
	// };
	//
	// /**
	//  * @brief Represents the structure of packet 0x11 used in haptic feedback for DualSense devices.
	//  *
	//  * This structure defines the format and content of the data packet identified as 0x11 in the
	//  * communication protocol with DualSense controllers. It includes fields required to properly
	//  * structure the packet headers and payload for device processing.
	//  *
	//  * Features:
	//  * - Contains a 6-bit packet identifier (PID) for recognizing the packet type.
	//  * - Includes metadata flags for additional control (bUnk and bSized).
	//  * - Supports a fixed data payload of 7 bytes to store the actual haptic feedback data.
	//  * - Provides a field for specifying the total length of the packet.
	//  *
	//  * The structure is tightly packed for alignment with the hardware protocol, ensuring the
	//  * efficient transmission of haptic commands.
	//  */
	// struct FPacket0X11 {
	// 	uint8 pid:6;
	// 	uint8 length;
	// 	uint8 data[64];
	// };

	/**
	 * @brief Represents the structure of packet 0x12 used in haptic feedback for DualSense devices.
	 *
	 * This structure is designed to define the format and content of the data packet identified as
	 * 0x12 in the communication protocol with DualSense controllers. It provides essential fields
	 * for the proper construction and transmission of haptic feedback commands.
	 *
	 * Features:
	 * - Includes a 6-bit packet identifier (PID) to recognize the packet type.
	 * - Contains control flags (bUnk and bSized) for indicating metadata about the packet.
	 * - Provides a field to specify the total length of the packet (Length).
	 * - Supports a fixed-size payload of 64 bytes to store the actual haptic feedback data.
	 *
	 * This structure is specifically aligned for compatibility with hardware-level communication
	 * protocols, ensuring accurate and efficient data transmission to the controller.
	 */
	// struct FPacket0X12 {
	// 	uint8 pid :6;
	// 	bool unk :1;
	// 	bool sized :1;
	// 	uint8 length;
	// 	uint8 data[64];
	// };

	uint8 Raw[142];
};

/**
 * @brief Represents the context and state of a connected device.
 *
 * This class is primarily used for managing the connection, data buffer, and
 * attributes of devices in the DualSense HID management system. Each device context
 * contains handles, connection details, and configuration needed for proper functionality.
 *
 * It is a crucial component for detecting, initializing, and managing devices
 * using related library systems.
 */
USTRUCT()
struct FDeviceContext
{
	GENERATED_BODY()
	/**
	 * @brief Raw device handle used for communication with a specific input/output hardware device.
	 *
	 * This handle represents a connection to the lower-level system or hardware interface
	 * and is primarily utilized for managing input/output operations, state retrieval,
	 * or other device-related activities.
	 *
	 * - The handle is created through platform-specific mechanisms, such as the `CreateHandle` function
	 *   in `UDualSenseHIDManager`.
	 * - It may represent a connection to a physical hardware device or similar system resources.
	 *
	 * @note Handle validity and state should always be verified before usage, as invalid or
	 * disconnected handles can result in undefined behavior.
	 * For instance, it may hold `INVALID_HANDLE_VALUE` when invalid or disconnected.
	 */
	FPlatformDeviceHandle Handle;
	/**
	 * @brief A platform-specific handle to manage interaction with audio devices.
	 *
	 * The AudioHandle variable represents a low-level handle for accessing
	 * and controlling audio hardware or audio-related platform resources.
	 * It provides an interface for abstracting platform-specific
	 * audio device interactions, allowing for unified handling across different systems.
	 *
	 * Key Points:
	 * - Allows communication and control of audio devices within the platform's ecosystem.
	 * - Encapsulates platform-dependent details for audio resource management.
	 * - Plays a crucial role in integrating platform audio capabilities into applications.
	 * - Used as a part of the audio subsystem to ensure efficient and seamless
	 *   interaction with the underlying hardware.
	 *
	 * This handle is specifically tailored to align with platform constraints and
	 * capabilities to ensure broad support for audio functionalities.
	 */
	FPlatformDeviceHandle AudioHandle;
	/**
	 * @brief Represents a file or resource path in the context of device management.
	 *
	 * This variable is used to store the location or identifier of a specific resource,
	 * configuration file, or executable path associated with the device or system operation.
	 *
	 * It plays a critical role in ensuring proper file system referencing and resource
	 * allocation within the management system.
	 */
	FString Path;
	/**
	 * @brief Internal data buffer for device communication.
	 *
	 * This buffer serves as a temporary storage space for handling input and output
	 * data during device interactions. It is specifically used in processing data
	 * exchange within the DualSense HID management framework.
	 *
	 * The size of the buffer is designed to accommodate the expected data payload
	 * during these communications, ensuring efficient handling of device protocols.
	 */
	unsigned char Buffer[78];
	/**
	 * @brief Stores the haptic audio data for DualSense devices.
	 *
	 * This buffer is used to handle and transfer haptic feedback audio data
	 * to a connected DualSense device, enabling advanced vibration and
	 * feedback mechanisms driven by audio signals.
	 */
	unsigned char BufferAudio[142];
	/**
	 * A fixed-size buffer for storing input or output data associated with a device context.
	 * This buffer is utilized for reading device input reports or for other data
	 * management tasks. Its size is defined as 78 bytes to accommodate device
	 * input requirements, particularly for devices connected over Bluetooth.
	 *
	 * Buffer is a critical component of FDeviceContext and is managed to ensure
	 * data consistency and proper memory zeroing upon disconnection or failure.
	 *
	 * @note The size of this buffer is determined by the specific device connection
	 *       type or input requirements, ensuring compatibility and sufficient
	 *       data handling capabilities.
	 */
	unsigned char BufferOutput[78];
	/**
	 * Indicates whether the device is connected.
	 *
	 * This boolean variable represents the connection state of a device.
	 * - `true` indicates the device is currently connected.
	 * - `false` indicates the device is not connected.
	 *
	 * It is used to manage and track the status of devices in the context
	 * of operations such as detection, initialization, and communication.
	 */
	bool IsConnected;
	/**
	 * Represents the output configuration for a device context, typically used
	 * to control advanced features of a connected DualSense controller.
	 *
	 * The FOutput structure manages outputs such as lightbar color, microphone
	 * light states, player indicator LEDs, rumble motor intensities, and haptic
	 * trigger effects. Additionally, it includes audio and general feature configurations.
	 *
	 * This variable is initialized within a device context and used in conjunction
	 * with runtime operations to apply desired settings to the connected controller.
	 */
	FOutputContext Output;
	/**
	 * Specifies the type of connection used by a device.
	 *
	 * ConnectionType is an instance of the EDeviceConnection enumeration,
	 * which defines the connection modes a device can use. Supported connection
	 * types include USB, Bluetooth, and Unrecognized.
	 *
	 * This variable is primarily used in the context of managing device communication,
	 * where it helps determine the appropriate handling for input and output operations.
	 *
	 * Possible values:
	 * - Usb: Represents devices connected via USB.
	 * - Bluetooth: Represents devices connected via Bluetooth.
	 * - Unrecognized: Represents devices with an unknown or unsupported connection type.
	 *
	 * In scenarios such as device discovery or IO operations, ConnectionType
	 * influences logic such as input report length, output buffering, and connection state updates.
	 */
	EDeviceConnection ConnectionType;
	/**
	 * @brief Represents the type of device in the context of DualSense HID management.
	 *
	 * DeviceType specifies the category or model of the connected device, enabling
	 * the system to adjust its behavior or configurations accordingly. Examples include
	 * default DualSense controllers, the Edge variation, and older DualShock models.
	 *
	 * It plays a pivotal role in distinguishing devices for operations such as feature
	 * initialization, compatibility checks, and tailored input/output processing.
	 */
	EDeviceType DeviceType;
	/**
	 * @brief Uniquely identifies an input device within the system.
	 *
	 * This identifier is used to distinguish individual devices connected to
	 * the system, ensuring that each device can be accurately managed and tracked.
	 *
	 * Essential for scenarios where multiple input devices are connected,
	 * enabling seamless interaction and device-specific operations.
	 */
	FInputDeviceId UniqueInputDeviceId;
	/**
	 * @brief Represents the sequence index for audio vibration feedback.
	 *
	 * Used to manage and track the current step in an audio-based vibration sequence
	 * within the device. This sequence enables precise synchronization of vibration
	 * effects with audio playback.
	 */
	uint8 AudioVibrationSequence = 0;

	FDeviceContext() : Handle(nullptr), AudioHandle(nullptr), Path{}, Buffer{}, BufferAudio{},
	                   BufferOutput{},
	                   IsConnected(false),
	                   ConnectionType(), DeviceType(),
	                   UniqueInputDeviceId()
	{
	}
};
