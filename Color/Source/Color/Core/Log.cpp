#include "ColorPCH.h"
#include "Log.h"

TScope<FLogger> GEngineLogger;
TScope<FLogger> GClientLogger;
TScope<FLogger> GAssertLogger;

void InitLog()
{
	GEngineLogger = MakeScope<FLogger>("ENGINE", FLoggerFileInfo("Log/Engine.log"));
	GClientLogger = MakeScope<FLogger>("CLIENT", FLoggerFileInfo("Log/Client.log"));
	GAssertLogger = MakeScope<FLogger>("ASSERT", FLoggerFileInfo("Log/Assert.log"));
}
