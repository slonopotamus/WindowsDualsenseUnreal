<h1 align="center">Dualsense Unreal Plugin</h1>

<p align="center">
Integrate all the features of Sony's DualSense™ and DualShock 4® controllers into your Unreal Engine project.
<br />
<br />
<a href="https://github.com/rafaelvaloto/WindowsDualsenseUnreal/issues">Report Bug</a>
·
<a href="https://github.com/rafaelvaloto/WindowsDualsenseUnreal/issues">Suggest a Feature
</a>
·
<a href="https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki">Documentation
</a>
</p>

<p align="center">
<a href="https://github.com/rafaelvaloto/WindowsDualsenseUnreal/releases">
    <img src="https://img.shields.io/github/v/release/rafaelvaloto/WindowsDualsenseUnreal?style=for-the-badge&logo=github" alt="Latest Release">
</a>
<a href="https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/rafaelvaloto/WindowsDualsenseUnreal?style=for-the-badge" alt="License">
</a>
<img src="https://img.shields.io/badge/Unreal%20Engine-5.2+-blue?style=for-the-badge&logo=unrealengine" alt="Unreal Engine 5.x">
<br/>
<img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Platform: Windows">
<img src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black" alt="Platform: Linux">
<img src="https://img.shields.io/badge/macOS-000000?style=for-the-badge&logo=apple&logoColor=white" alt="Platform: macOS">
<img src="https://img.shields.io/badge/PlayStation-0070D1?style=for-the-badge&logo=playstation&logoColor=white" alt="Platform: PlayStation">
</p>

## Disclaimer and Trademarks

"PlayStation", "PlayStation Family Mark", "PS5 logo", "PS5", "DualSense" and "DualShock" are registered trademarks or trademarks of Sony Interactive Entertainment Inc. "SONY" is a registered trademark of Sony Corporation. "Windows" is a registered trademark of Microsoft Corporation. "Linux" is a registered trademark of Linus Torvalds. "Mac" is a registered trademark of Apple Inc.

The author of this project is not affiliated, associated, authorized, endorsed by, or in any way officially connected with Sony Interactive Entertainment Inc., Microsoft Corporation, Apple Inc., or Linus Torvalds. This is an independent and unofficial project.

## 📖 About the Project

Built with a cross-platform architecture, this Unreal Engine plugin provides a unified solution for integrating DualSense™ (PlayStation 5) and DualShock 4® (PlayStation 4) controllers. It delivers native support on PC while being architected for easy compilation across other Unreal Engine platforms, including PlayStation®. This asset provides direct API access to the complete feature set of each controller, including the revolutionary Haptic Feedback and Adaptive Triggers of the DualSense™. All features are exposed through a clean and well-documented function library for both Blueprint and C++.

Designed to bridge the gap left by generic controller support, this asset empowers developers to implement the high-fidelity, immersive feedback that makes Sony's controllers unique.

## ✨ Features
* 🏗️ **Extensible Multi-Platform Architecture**: Its flexible design enables compilation across all Unreal Engine platforms. Supporting new hardware is as simple as implementing the connection interface.
* 🔌 **Dynamic Connection (Hot-Swap)**: Automatically detects controller connection and disconnection, even during gameplay.
* ⚡ **Optimized for Multiplayer**: High-performance architecture with minimal impact on network games.
* 🎮 **Seamless Input Integration**: Coexists perfectly with Unreal Engine's native input managers (like Enhanced Input) and other gamepad plugins, preventing conflicts between devices.
* 🎯 **Adaptive Triggers**: Full control over resistance, effect, and vibration on R2/L2 triggers.
* 🔊 **Audio-Based Vibration**: Synchronize the controller's haptic feedback with any in-game audio source.
* 💡 **Lightbar Control**: Dynamically change the controller's LED color.
* 🎤 **Microphone and Audio**: Manage the mute button LED, speaker volume, and headset audio.
* ⚙️ **Force Feedback**: Native integration with Unreal Engine's Force Feedback system for standard motor vibration.
* 🎮 **Multi-Controller Support**: Manage up to 4 controllers simultaneously.

## 🎮 Example Project: Arena Shooter UE 5.6

To demonstrate the practical use of the **Dualsense Unreal Plugin**, a sample project has been developed using the [*Arena Shooter* template](https://www.unrealengine.com/marketplace/en-US/product/arena-shooter-template) and upgraded to Unreal Engine 5.6. This project integrates key features of the DualSense controller to enhance the player's experience.

  
![Arena Shooter UE 5.6](Images/ArenaBanner.jpg)

### Implemented Features

In this sample, the following DualSense functionalities were integrated to provide a more immersive gameplay experience:

* 🔫 **Automatic Gun**: Experience fully automatic firing with appropriate haptic feedback, simulating realistic weapon recoil and vibration.
* 🎯 **Semi-Automatic Gun**: Engage in precise shooting with semi-automatic weapons, where each shot provides distinct haptic sensations and trigger resistance.
* 💥 **Vibration on Player Hit & Visual Feedback**: Feel the impact! The controller vibrates dynamically when the player takes damage, complemented by on-screen visual feedback for enhanced immersion and immediate awareness.
* 💡 **LED Color Change on Player Hit**: The DualSense controller's LED light dynamically changes color when the player is hit, offering an immediate and intuitive visual cue of damage taken, enhancing situational awareness.

### Where to Download

You can download the *Arena Shooter UE 5.6* with the DualSense integration directly from link.

- [**Download the example project for the editor here**](https://drive.google.com/file/d/1oornHLpanEoHoDPRL1jfF_hvU17phsbp/view?usp=drive_link)
- [**Download the compiled version of the example project here**](https://drive.google.com/file/d/1H6lvd0Ta-M4Pwtu2w2s4YyTaPobaqKfc/view?usp=drive_link)

### 🎓 Hands-On Tutorial

We've created a detailed, step-by-step tutorial that breaks down the entire implementation within the example project. It's the perfect guide to get you started.

* 🎯 **Gyroscope Aiming**: A complete tutorial on how to implement a precise and responsive Aim Down Sights (ADS) gyro aiming system using the Arena Shooter template.
    ➡️ **[[Read the Gyroscope Aiming Tutorial]](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/Example-Project:-Gyroscope-Aiming)**

* 🔫 **Arena Shooter**: An example using the Arena Shooter template that implements adaptive triggers for automatic/semi-automatic weapons and haptic feedback for player damage.
    ➡️ **[[Read the Arena Shooter Tutorial]](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/Example-Project:-Arena-Shooter-Tutorial)**

## 🎮 Example Project: Parrot Game Sample

To demonstrate the practical use of the **Windows Dualsense Unreal Plugin**, a sample project has been developed using the [*Parrot Game Sample*](https://dev.epicgames.com/documentation/en-us/unreal-engine/parrot-game-sample-for-unreal-engine) from Epic Games. This project integrates key features of the DualSense controller to enhance the player's experience.

![Parrot Game Sample](Images/parrot-game-sample-banner.jpg)


### Implemented Features

In this sample, the following DualSense functionalities were integrated to provide a more immersive gameplay experience:

* **Visual Feedback**: The controller's Lightbar is used to provide real-time visual feedback to the player, changing colors and effects according to in-game events.
* **Vibration (Force Feedback)**: The native Force Feedback system was used to create detailed vibration effects, increasing immersion during key moments of gameplay.

> 💡 **Pro Tip for Deeper Immersion**: For an enhanced audio experience, connect the controller via USB and plug a headset directly into it. This setup also works with a Bluetooth connection, offering greater flexibility.

This project serves as a practical guide for developers who want to see the plugin in action and learn how to integrate the unique features of the DualSense controller into their own games.


### Where to Download

You can download the *Parrot Game Sample* with the DualSense integration directly from link.

- [**Download the example project for the editor here**](https://drive.google.com/file/d/198Dko7ZwIX1vz9jw7RtYp4arY9Qp5bJ4/view?usp=drive_link)
- [**Download the compiled version of the example project here**](https://drive.google.com/file/d/144hM71xZufBe29UzpTNQ1rRe0AYWC-Ka/view?usp=drive_link)

## 🎮 Example Project: Audio-Based Vibration

This project demonstrates the Advanced Audio Vibration, a native feature of the DualSense controller. It serves as a simple playlist to showcase how music, footsteps, and other sound effects can be translated directly into high-fidelity haptics when the controller is connected via USB.

### Implemented Features

Native Haptic Generation: This example uses the advanced internal capabilities of the DualSense to generate vibrations directly from a sound source.

Simple Activation: The feature is easily enabled through the Device Settings node by setting the VibrationMode to Audio based vibration. This requires a USB connection to function.

Advanced Practice: For even more detailed effects, the underlying audio classes in Unreal Engine allow you to apply Low-Pass and High-Pass filters. This technique can isolate bass frequencies (for rumbles) from high-frequency sounds (for sharp textures), providing a more granular and immersive haptic experience.

- [**Download the example project for the editor here**](https://drive.google.com/file/d/1D4g_A78xBMdLyDy0xHdr1TbSuHdSuHGn/view?usp=drive_link)

## 🚀 Getting Started

### Prerequisites

* **Unreal Engine**: 5.2 or higher.
* **Operating System**: Windows 10 or 11.
* **Controller**: DualSense™ or DualShock 4®.

### Quick Installation

1.  Go to the official plugin page on the Unreal Engine Marketplace (FAB): [Plugin Page - FAB](https://www.fab.com/listings/e77a8f1d-8bbe-4673-a5ae-7f222c8c0960)
2.  Click **Install** or **Add to Project** and select your Unreal Engine project.
3.  Activate the plugin in Unreal Engine:
    * Open your project.
    * Go to `Edit > Plugins`.
    * Search for **Windows Dualsense Plugin** and check the box.
4.  Restart Unreal Engine when prompted.

## 💻 Basic Usage

The plugin exposes all functionality through static Blueprint function libraries, meaning you can call methods from anywhere without needing to add components.

### Blueprint Function Libraries

The functions are divided into two main categories for easy access:

* **Sony Gamepad**: Contains management methods common to Sony controllers (DualShock and DualSense), such as LED control, gyroscope, battery level, etc.
* **DualSense Effects**: Contains methods specific to DualSense exclusive features, such as Adaptive Triggers configuration.
  
Call functions directly to control DualSense features. Some available effects include:

* 🐎 **Galloping**: Simulates a horse's trot.
* 💪 **Resistance**: Applies constant opposing force when pressing the trigger.
* 🔫 **Weapon**: Creates a recoil effect for semi-automatic weapons.
* 🔥 **Automatic Gun**: Vibrates rapidly to simulate an automatic weapon.  

### 📚 For the full documentation, please see the **[Wiki](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki)**.

## 🛠️ Extending for Other Platforms (e.g., PlayStation)

The plugin has been designed with an extensible architecture, allowing developers with access to other platform SDKs (such as the official Sony PlayStation® SDK) to integrate them with minimal effort.

The low-level hardware communication is abstracted through the `IPlatformHardwareInfoInterface`. The default implementation for Windows and Linux uses the HID API to communicate with the controllers.

For licensed developers, extending the plugin involves these steps:

1.  **Create a new implementation class**: Create a new C++ class that inherits from `IPlatformHardwareInfoInterface` and implements its virtual methods using the specific SDK's functions.
2.  **Modify the Singleton**: In the `IPlatformHardwareInfoInterface.cpp` file, include the header for your new class and instantiate it within the appropriate conditional compilation block.

> ➡️ **For a detailed, step-by-step guide on how to add support for a new platform, [please see our tutorial on the Wiki](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/🎮-Extending-the-Plugin-for-Other-Platforms).**

## 🤝 How to Contribute

Contributions are welcome! If you have ideas, suggestions, or bug fixes, feel free to open an *Issue* or submit a *Pull Request*.

## 📄 License

This project is distributed under the MIT License. See the `LICENSE` file for more details.

## ⭐ Credits and Acknowledgments

Special thanks to everyone who has contributed with suggestions, reported bugs, and offered implementation improvements. Thanks also to the developers of the libraries used as inspiration and reference for creating this project.

A special thanks as well to the Epic Games team for creating and providing the *Parrot Game Sample*, which served as an excellent foundation for the example project demonstrating this plugin's features.

* [DualSense on Windows API](https://github.com/Ohjurot/DualSense-Windows)
* [Nielk1 on GIST](https://gist.github.com/Nielk1/6d54cc2c00d2201ccb8c2720ad7538db)
* [DualSenseAPI](https://github.com/BadMagic100/DualSenseAPI/tree/master)
* [flok pydualsense](https://github.com/flok/pydualsense)

