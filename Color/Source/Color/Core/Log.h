#pragma once

#include "Core/Logger.h"
#include "Templates/Scope.h"

// The generic logger used to log any sort of information by the engine.
extern TScope<FLogger> GEngineLogger;

// The generic logger used to log any sort of information by the game/client/application.
extern TScope<FLogger> GClientLogger;

// The logger used to log assertion (check/verify/unreachable) failures.
extern TScope<FLogger> GAssertLogger;

void InitLog();

#define CL_CORE_TRACE(...)   CL_LOGGER_TRACE(GEngineLogger, __VA_ARGS__)
#define CL_CORE_INFO(...)    CL_LOGGER_INFO(GEngineLogger, __VA_ARGS__)
#define CL_CORE_WARN(...)    CL_LOGGER_WARN(GEngineLogger, __VA_ARGS__)
#define CL_CORE_ERROR(...)   CL_LOGGER_ERROR(GEngineLogger, __VA_ARGS__)
#define CL_CORE_FATAL(...)   CL_LOGGER_FATAL(GEngineLogger, __VA_ARGS__)

#define CL_TRACE(...)        CL_LOGGER_TRACE(GClientLogger, __VA_ARGS__)
#define CL_INFO(...)         CL_LOGGER_INFO(GClientLogger, __VA_ARGS__)
#define CL_WARN(...)         CL_LOGGER_WARN(GClientLogger, __VA_ARGS__)
#define CL_ERROR(...)        CL_LOGGER_ERROR(GClientLogger, __VA_ARGS__)
#define CL_FATAL(...)        CL_LOGGER_FATAL(GClientLogger, __VA_ARGS__)

#define CL_ASSERT_TRACE(...) CL_LOGGER_TRACE(GAssertLogger, __VA_ARGS__)
#define CL_ASSERT_INFO(...)  CL_LOGGER_INFO(GAssertLogger, __VA_ARGS__)
#define CL_ASSERT_WARN(...)  CL_LOGGER_WARN(GAssertLogger, __VA_ARGS__)
#define CL_ASSERT_ERROR(...) CL_LOGGER_ERROR(GAssertLogger, __VA_ARGS__)
#define CL_ASSERT_FATAL(...) CL_LOGGER_FATAL(GAssertLogger, __VA_ARGS__)
