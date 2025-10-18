// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include "Subsystems/AudioHapticsListener.h"

class FHapticsRegistry final : public TSharedFromThis<FHapticsRegistry>
{
    /**
     * Retrieves the singleton instance of the FHapticsRegistry.
     *
     * This method ensures that there is only a single shared instance
     * of FHapticsRegistry available. If the instance has not been created yet,
     * it initializes and returns a newly created shared instance. This is a thread-safe way
     * to manage a centralized registry for haptics-related operations.
     *
     * @return A shared pointer to the singleton instance of FHapticsRegistry.
     */
public:
    static TSharedPtr<FHapticsRegistry> Get();
    /**
     * Creates and associates a new audio haptics listener for a specified input device and submix.
     *
     * This method ensures that only one listener is active for the given device. If a listener already exists,
     * it removes the previous association before creating and registering a new listener for the specified submix.
     *
     * @param DeviceId The unique identifier of the input device for which the listener is created.
     * @param Submix A pointer to the sound submix to which the audio haptics listener will be bound.
     *               If this is null, the method will return without taking any action.
     */
    void CreateListenerForDevice(const FInputDeviceId& DeviceId, USoundSubmix* Submix);
    /**
     * Removes the audio haptics listener for the specified input device.
     *
     * This method ensures that any existing listener associated with the given
     * device ID is properly removed and unregistered. If no listener exists for
     * the provided device ID, the method takes no action.
     *
     * @param DeviceId The unique identifier of the input device whose associated
     *                 haptics listener is to be removed.
     */
    void RemoveListenerForDevice(const FInputDeviceId& DeviceId);
    /**
     * Destructor for the FHapticsRegistry class.
     *
     * Cleans up and releases all resources managed by this instance of FHapticsRegistry.
     * Specifically, it iterates through all registered listeners in the ControllerListeners map
     * and ensures that all associated listeners are properly removed.
     *
     * This method guarantees that the resources used for managing haptics-related listeners are
     * de-allocated in a safe and orderly manner, avoiding resource leaks or dangling listeners.
     */
    virtual ~FHapticsRegistry();
    /**
     * Holds the singleton instance of FHapticsRegistry.
     *
     * This static member variable is used to ensure that only one instance
     * of the FHapticsRegistry class exists during the application's runtime.
     * It is initialized the first time the Get() method is called and remains
     * valid until the application terminates or the instance is explicitly destroyed.
     */
private:
    static TSharedPtr<FHapticsRegistry> Instance;
    /**
     * Maps input device identifiers to their associated audio haptics listeners.
     *
     * This data structure is responsible for managing the relationship between input devices
     * (identified by FInputDeviceId) and their corresponding haptics listeners
     * (represented by TSharedPtr<FAudioHapticsListener>). It ensures that each device
     * has a corresponding listener, allowing for device-specific haptics handling.
     *
     * The map is used primarily within the FHapticsRegistry class to register, retrieve,
     * and remove audio haptics listeners as devices are added or removed.
     */
    TMap<FInputDeviceId, TSharedPtr<FAudioHapticsListener>> ControllerListeners;
};
