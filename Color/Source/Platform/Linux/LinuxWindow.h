#pragma once

#include "Platform/Generic/GenericWindow.h"

class FLinuxWindow : public FGenericWindow
{
public:
	FLinuxWindow(const FWindowProps& Props = {});
	virtual ~FLinuxWindow() = default;
};
