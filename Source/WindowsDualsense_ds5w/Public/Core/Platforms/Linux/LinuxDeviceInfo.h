
#pragma once
#include "Core/Interfaces/PlatformHardwareInfoInterface.h"

class FLinuxDeviceInfo : public IPlatformHardwareInfoInterface
{
public:
	virtual void Read(FDeviceContext* Context) override;
	virtual void Write(FDeviceContext* Context) override;
	virtual void Detect(TArray<FDeviceContext>& Devices) override;
	virtual bool CreateHandle(FDeviceContext* Context) override;
	virtual void InvalidateHandle(FDeviceContext* Context) override;
};
