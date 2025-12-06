// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include <cmath>

class FMadgwickAhrs
{
public:
	explicit FMadgwickAhrs(const float Beta = 0.08f);

	// gx,gy,gz in rad/s, ax,ay,az in m/s^2, dt in seconds
	void UpdateImu(float gx, float gy, float gz, float ax, float ay, float az, float dt);
	void SetBeta(const float BetaValue);
	void GetQuaternion(float& Nq0, float& Nq1, float& Nq2, float& Nq3) const;
	void Reset();

private:
	float Beta;
	float q0, q1, q2, q3;
};
