// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "Core/Interfaces/PlatformHardwareInfoInterface.h"

class FLinuxDeviceInfo : public IPlatformHardwareInfoInterface
{
	/**
	 * Reads data from the device associated with the given context.
	 * This method processes the input buffer based on the device and connection types.
	 *
	 * @param Context Pointer to the FDeviceContext structure that contains the
	 *                device handle, connection type, device type, and buffer information.
	 *                If the context or its handle is invalid, the method will return without performing any action.
	 */
public:
	virtual void Read(FDeviceContext* Context) override;
	/**
	 * Writes data to the device associated with the given context.
	 * This method processes the output buffer based on the device and connection types
	 * and attempts to write the appropriate report length to the device.
	 *
	 * @param Context Pointer to the FDeviceContext structure that contains the
	 *                device handle, connection type, device type, and buffer information.
	 *                If the context or its handle is invalid, the method will return without performing any action.
	 */
	virtual void Write(FDeviceContext* Context) override;
	/**
	 * Detects and enumerates supported devices connected to the system.
	 * The method scans for devices, validates them against supported product IDs,
	 * and populates the provided array with the details of recognized devices.
	 *
	 * @param Devices Reference to an array of FDeviceContext that will be populated
	 *                with detected device information. The array is cleared at the
	 *                beginning of the method to ensure only current devices are listed.
	 */
	virtual void Detect(TArray<FDeviceContext>& Devices) override;
	/**
	 * Creates and assigns a device handle based on the given context.
	 * This method attempts to open a device handle specified by the path in the context
	 * and sets it to non-blocking mode.
	 *
	 * @param Context Pointer to the FDeviceContext structure that contains the device path.
	 *                The path must be valid and not empty for the handle creation to succeed.
	 *                If the path is invalid or an error occurs during handle creation,
	 *                the method returns false and does not assign a handle to the context.
	 *
	 * @return True if the device handle was successfully created and assigned; otherwise, false.
	 */
	virtual bool CreateHandle(FDeviceContext* Context) override;
	/**
	 * Invalidates the device handle associated with the given context.
	 * This method closes the existing device handle, resets the handle and connection status,
	 * clears the path, and zeroes out the context's buffers to ensure a clean state.
	 *
	 * @param Context Pointer to the FDeviceContext structure that contains the
	 *                device handle and buffer information. The context must be valid,
	 *                and the handle must be non-null for this method to take action.
	 */
	virtual void InvalidateHandle(FDeviceContext* Context) override;
};
