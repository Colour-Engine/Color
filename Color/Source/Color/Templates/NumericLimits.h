#pragma once

#include "Misc/IntTypes.h"

#include <stdint.h>
#include <float.h>

template <typename T>
struct TNumericLimits
{
};

template <>
struct TNumericLimits<bool>
{
	static constexpr bool Min() { return false; }
	static constexpr bool Max() { return true; }
};

template <>
struct TNumericLimits<int8>
{
	static constexpr int8 Min() { return INT8_MIN; }
	static constexpr int8 Max() { return INT8_MAX; }
};

template <>
struct TNumericLimits<int16>
{
	static constexpr int16 Min() { return INT16_MIN; }
	static constexpr int16 Max() { return INT16_MAX; }
};

template <>
struct TNumericLimits<int32>
{
	static constexpr int32 Min() { return INT32_MIN; }
	static constexpr int32 Max() { return INT32_MAX; }
};

template <>
struct TNumericLimits<int64>
{
	static constexpr int64 Min() { return INT64_MIN; }
	static constexpr int64 Max() { return INT64_MAX; }
};

template <>
struct TNumericLimits<uint8>
{
	static constexpr uint8 Min() { return 0; }
	static constexpr uint8 Max() { return UINT8_MAX; }
};

template <>
struct TNumericLimits<uint16>
{
	static constexpr uint16 Min() { return 0; }
	static constexpr uint16 Max() { return UINT16_MAX; }
};

template <>
struct TNumericLimits<uint32>
{
	static constexpr uint32 Min() { return 0; }
	static constexpr uint32 Max() { return UINT32_MAX; }
};

template <>
struct TNumericLimits<uint64>
{
	static constexpr uint64 Min() { return 0; }
	static constexpr uint64 Max() { return UINT64_MAX; }
};

template <>
struct TNumericLimits<float>
{
	static constexpr float Min() { return FLT_MIN; }
	static constexpr float Max() { return FLT_MAX; }
};

template <>
struct TNumericLimits<double>
{
	static constexpr double Min() { return DBL_MIN; }
	static constexpr double Max() { return DBL_MAX; }
};

template <>
struct TNumericLimits<long double>
{
	static constexpr long double Min() { return LDBL_MIN; }
	static constexpr long double Max() { return LDBL_MAX; }
};
