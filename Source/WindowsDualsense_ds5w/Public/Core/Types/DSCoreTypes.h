// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once
#include <cstdint>

namespace DSCoreTypes
{
	typedef std::int8_t int8;
	typedef std::uint8_t uint8;

	typedef std::int16_t int16;
	typedef std::uint16_t uint16;

	typedef std::int32_t int32;
	typedef std::uint32_t uint32;

	typedef std::int64_t int64;
	typedef std::uint64_t uint64;

	typedef float float32;
	typedef double float64;

	namespace LedMasks
	{
		static constexpr uint8 Left = 0x01;
		static constexpr uint8 MiddleLeft = 0x02;
		static constexpr uint8 Middle = 0x04;
		static constexpr uint8 MiddleRight = 0x08;
		static constexpr uint8 Right = 0x10;
	} // namespace LedMasks

	namespace InputMasks
	{
		// (Face Buttons)
		namespace Face
		{
			static constexpr uint8 Square = 0x10;
			static constexpr uint8 Cross = 0x20;
			static constexpr uint8 Circle = 0x40;
			static constexpr uint8 Triangle = 0x80;
		} // namespace Face

		// (D-Pad)
		namespace DPad
		{
			static constexpr uint8 Up = 0x08;
			static constexpr uint8 Right = 0x04;
			static constexpr uint8 Down = 0x02;
			static constexpr uint8 Left = 0x01;
		} // namespace DPad

		// Triggers (Shoulders)
		namespace Shoulder
		{
			static constexpr uint8 L1 = 0x01;
			static constexpr uint8 R1 = 0x02;
			static constexpr uint8 L2 = 0x04;
			static constexpr uint8 R2 = 0x08;
		} // namespace Shoulder

		// Btn Menu
		namespace Menu
		{
			static constexpr uint8 Share = 0x10;    // Select / Create
			static constexpr uint8 Options = 0x20;  // Start
			static constexpr uint8 PS = 0x01;       // Logo
			static constexpr uint8 TouchPad = 0x02; // Click Pad
			static constexpr uint8 Mute = 0x04;     // Mic Button
		} // namespace Menu

		// Sticks (Click L3/R3)
		namespace Stick
		{
			static constexpr uint8 L3 = 0x40;
			static constexpr uint8 R3 = 0x80;
		} // namespace Stick

		// Extras (DualSense Edge / Pro Features)
		namespace Extra
		{
			static constexpr uint8 FnLeft = 0x10;      // FN1
			static constexpr uint8 FnRight = 0x20;     // FN2
			static constexpr uint8 PaddleLeft = 0x40;  // Back Button Left
			static constexpr uint8 PaddleRight = 0x80; // Back Button Right
		} // namespace Extra
	} // namespace InputMasks
} // namespace DSCoreTypes
