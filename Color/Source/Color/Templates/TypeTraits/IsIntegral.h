#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/TypeTraits/RemoveCv.h"
#include "Misc/IntTypes.h"

template <typename T>
struct TIsIntegral : FFalseType
{
};

template <> struct TIsIntegral<bool>     : FTrueType { };
template <> struct TIsIntegral<char8_t>  : FTrueType { };
template <> struct TIsIntegral<char16_t> : FTrueType { };
template <> struct TIsIntegral<char32_t> : FTrueType { };
template <> struct TIsIntegral<wchar_t>  : FTrueType { };
template <> struct TIsIntegral<int8>     : FTrueType { };
template <> struct TIsIntegral<int16>    : FTrueType { };
template <> struct TIsIntegral<int32>    : FTrueType { };
template <> struct TIsIntegral<int64>    : FTrueType { };
template <> struct TIsIntegral<uint8>    : FTrueType { };
template <> struct TIsIntegral<uint16>   : FTrueType { };
template <> struct TIsIntegral<uint32>   : FTrueType { };
template <> struct TIsIntegral<uint64>   : FTrueType { };

template <typename T>
inline constexpr bool VIsIntegral = TIsIntegral<T>::Value;
