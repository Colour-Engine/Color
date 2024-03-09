#include "ColorPCH.h"
#include "Log.h"

FLogger GEngineLogger =
{
	"COLOR",
	 FLoggerFileInfo
	 (
		 "Logs/Engine.log"
	 )
};

FLogger GClientLogger =
{
	"APP",
	 FLoggerFileInfo
	 (
		 "Logs/Client.log"
	 )
};

FLogger GAssertLogger =
{
	"ASSERT",
	 FLoggerFileInfo
	 (
		 "Logs/Assert.log"
	 )
};
