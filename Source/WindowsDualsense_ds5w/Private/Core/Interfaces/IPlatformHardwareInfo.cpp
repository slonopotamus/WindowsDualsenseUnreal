// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Core/Interfaces/IPlatformHardwareInfo.h"

#if PLATFORM_WINDOWS
#include "Implementations/Platforms/Windows/WindowsDeviceInfo.h"
#elif PLATFORM_MAC || PLATFORM_LINUX
#include "Implementations/Platforms/Commons/CommonsDeviceInfo.h"
#elif PLATFORM_SONY
#include "Implementations/Platforms/Sony/FNullHardwareInterface.h"
#endif

TUniquePtr<IPlatformHardwareInfo> IPlatformHardwareInfo::PlatformInfoInstance = nullptr;

/**
 * Retrieves a reference to the platform-specific hardware information interface instance.
 * If the instance does not already exist, it is initialized based on the current platform.
 *
 * - For Windows, the instance is initialized using HID (Human Interface Device) for
 *   DualSense controller support.
 * - For other platforms, the interface is currently not supported and will return nullptr.
 *
 * @return A reference to the platform-specific hardware information interface instance.
 *         If the platform is unsupported, dereferencing will result in undefined behavior as nullptr is returned.
 */
IPlatformHardwareInfo& IPlatformHardwareInfo::Get()
{
	if (!PlatformInfoInstance)
	{
		// Platform-specific initialization of hardware info interface
		// Windows: Uses HID (Human Interface Device) for DualSense controller support
		// Other platforms: Currently not supported (returns nullptr)
		//
		// Usage:
		// - PLATFORM_WINDOWS: Windows-specific implementation using hidapi
		// - PLATFORM_MAC: Reserved for future macOS implementation using hidapi
		// - PLATFORM_LINUX: Reserved for future Linux implementation using hidapi
		// - PLATFORM_SONY: Reserved for future PlayStation implementation
		//
#if PLATFORM_WINDOWS
		PlatformInfoInstance = MakeUnique<FWindowsDeviceInfo>();
#elif PLATFORM_MAC || PLATFORM_LINUX
		PlatformInfoInstance = MakeUnique<FCommonsDeviceInfo>();
#elif PLATFORM_SONY
		// Note: PLATFORM_SONY implementation is reserved for licensed PlayStation developers only
		// Example:
		// To implement for PlayStation platforms, create a class in Platforms/Sony directory:
		// class FPlayStationDeviceInfo: public IIPlatformHardwareInfo
		// {
		//	// Implement required interface methods
		// };
		PlatformInfoInstance = MakeUnique<FPlayStationDeviceInfo>();
#endif
	}
	return *PlatformInfoInstance;
}
