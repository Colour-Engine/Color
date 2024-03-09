#pragma once

#include "Core/Log.h"

#include "Misc/MiscDefines.h"
#include "Misc/ExitCode.h"

#ifndef CL_SHIPPING
	#define CL_AssertHelper() { CL_PLATFORM_DEBUGBREAK(); }
#else
	#define CL_AssertHelper() { RtExit(ExitCode::AssertFail); }
#endif

#define CL_AssertBaseMacro(Type, Expression) \
if (!(Expression)) \
{ \
	CL_ASSERT_ERROR(Type " failed! Expression was '" #Expression "'."); \
	CL_AssertHelper(); \
}

#define CL_AssertaBaseMacro(Type, Expression) \
if (!(Expression)) \
{ \
	CL_ASSERT_ERROR(Type " failed in file '" __FILE__ "' at line " CL_STRINGIFY(__LINE__) "! Expression was '" #Expression "'."); \
	CL_AssertHelper(); \
}

#define CL_AssertfBaseMacro(Type, Expression, ...) \
if (!(Expression)) \
{ \
	CL_ASSERT_ERROR(Type " failed! " __VA_ARGS__); \
	CL_AssertHelper(); \
}

#define CL_UnreachableBaseMacro() \
if (!(Expression)) \
{ \
	CL_ASSERT_ERROR("Unreachable point in file '" __FILE__ "' at line " CL_STRINGIFY(__LINE__) " was hit!"); \
	CL_AssertHelper(); \
}

#define verify(Expression)       CL_AssertBaseMacro("Verification", Expression)
#define verifya(Expression)      CL_AssertaBaseMacro("Verification", Expression)
#define verifyf(Expression, ...) CL_AssertfBaseMacro("Verification", Expression, __VA_ARGS__)
#define always_unreachable()     CL_UnreachableBaseMacro()

#ifndef CL_SHIPPING
	#define CL_DEBUGBREAK()         CL_PLATFORM_DEBUGBREAK()
	#define check(Expression)       CL_AssertBaseMacro("Check", Expression)
	#define checka(Expression)      CL_AssertaBaseMacro("Check", Expression)
	#define checkf(Expression, ...) CL_AssertfBaseMacro("Check", Expression, __VA_ARGS__)
	#define unreachable()           CL_UnreachableBaseMacro()
#else
	#define CL_DEBUGBREAK()
	#define check(Expression)
	#define checka(Expression)
	#define checkf(Expression, ...)
	#define unreachable()
#endif
