#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/TypeTraits/RemoveCv.h"
#include "Misc/IntTypes.h"

template <typename T>
struct TIsFloatingPoint : FFalseType
{
};

template <> struct TIsFloatingPoint<float> : FTrueType { };
template <> struct TIsFloatingPoint<double> : FTrueType { };
template <> struct TIsFloatingPoint<long double> : FTrueType { };

template <typename T>
inline constexpr bool VIsFloatingPoint = TIsFloatingPoint<T>::Value;
