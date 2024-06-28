#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T> struct TIsSigned              : FFalseType { };
template <>           struct TIsSigned<int8>        : FTrueType { };
template <>           struct TIsSigned<int16>       : FTrueType { };
template <>           struct TIsSigned<int32>       : FTrueType { };
template <>           struct TIsSigned<int64>       : FTrueType { };
template <>           struct TIsSigned<float>       : FTrueType { };
template <>           struct TIsSigned<double>      : FTrueType { };
template <>           struct TIsSigned<long double> : FTrueType { };

template <typename T>
inline constexpr bool VIsSigned = TIsSigned<T>::Value;
