#pragma once

#include "Core/Application.h"

class FSandboxApp : public FApplication
{
public:
	FSandboxApp(const FCommandLine& InCommandLine);
	~FSandboxApp() override;
};
