#pragma once

#include "Misc/IntTypes.h"

template <typename T> struct MakeSigned { typedef T Type; };
template <>           struct MakeSigned<uint8>  { typedef int8  Type; };
template <>           struct MakeSigned<uint16> { typedef int16 Type; };
template <>           struct MakeSigned<uint32> { typedef int32 Type; };
template <>           struct MakeSigned<uint64> { typedef int64 Type; };

template <typename T>
using TMakeSigned = typename MakeSigned<T>::Type;
