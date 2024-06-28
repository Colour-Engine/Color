#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T, typename U> struct TIsSame       : FFalseType { };
template <typename T>             struct TIsSame<T, T> : FTrueType  { };

template <typename T, typename U>
inline constexpr bool VIsSame = TIsSame<T, U>::Value;
