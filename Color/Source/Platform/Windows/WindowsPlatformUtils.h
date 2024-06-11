#pragma once

#include "Platform/Generic/GenericPlatformUtils.h"

class FWindowsPlatformUtils : public FGenericPlatformUtils
{
public:
	virtual glm::ivec2 GetMonitorSize_Impl() const override;
};
