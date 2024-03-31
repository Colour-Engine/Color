#pragma once

#include "Platform/Generic/GenericWindow.h"

class FWindowsWindow : public FGenericWindow
{
public:
	FWindowsWindow(const FWindowProps& Props = {});
	virtual ~FWindowsWindow() = default;
};
