#pragma once

#include "Utils/PlatformUtils.h"

class FGenericPlatformUtils : public FPlatformUtils
{
protected:
	virtual float GetTime_Impl() const override;
};
