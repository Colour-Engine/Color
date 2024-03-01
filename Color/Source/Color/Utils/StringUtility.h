#pragma once

#include "Misc/IntTypes.h"

// Expected static methods that the implementation must suffice (may be updated as time goes on):
//   - uint_t Len(const T*)
//   - int32 Cmp(const T*, const T*)
template <typename T>
struct TStringUtility
{
};

template <>
struct TStringUtility<char>
{
	static uint_t Len(const char* String);
	static int32  Cmp(const char* LHS, const char* RHS);
};
