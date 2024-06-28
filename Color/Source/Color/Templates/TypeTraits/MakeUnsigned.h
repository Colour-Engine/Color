#pragma once

#include "Misc/IntTypes.h"

template <typename T> struct MakeUnsigned { typedef T Type; };
template <>           struct MakeUnsigned<int8>  { typedef uint8  Type; };
template <>           struct MakeUnsigned<int16> { typedef uint16 Type; };
template <>           struct MakeUnsigned<int32> { typedef uint32 Type; };
template <>           struct MakeUnsigned<int64> { typedef uint64 Type; };

template <typename T>
using TMakeUnsigned = typename MakeUnsigned<T>::Type;
