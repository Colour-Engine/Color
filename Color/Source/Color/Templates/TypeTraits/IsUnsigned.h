#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T> struct TIsUnsigned         : FFalseType { };
template <>           struct TIsUnsigned<uint8>  : FTrueType { };
template <>           struct TIsUnsigned<uint16> : FTrueType { };
template <>           struct TIsUnsigned<uint32> : FTrueType { };
template <>           struct TIsUnsigned<uint64> : FTrueType { };

template <typename T>
inline constexpr bool VIsUnsigned = TIsUnsigned<T>::Value;
