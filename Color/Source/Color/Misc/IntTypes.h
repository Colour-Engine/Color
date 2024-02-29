#pragma once

typedef signed char        int8;
typedef signed short int   int16;
typedef signed int         int32;
typedef signed long long   int64;

typedef unsigned char      uint8;
typedef unsigned short int uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

typedef signed char        sbyte;
typedef unsigned char      ubyte;

// Either int32 or int64, depending on the target architecture. Because Color only supports x64 as of now, it will always be 64-bits.
// Will be the same type as ssize_t would be.
typedef signed long long   sint_t;

// Either uint32 or uint64, depending on the target architecture. Because Color only supports x64 as of now, it will always be 64-bits.
// Will be the same type as size_t would be.
typedef unsigned long long uint_t;

// Type of the 'nullptr'. Can be used as a parameter to handle cases where nullptr was passed into a method, like the STL does.
typedef decltype(nullptr)  TYPE_OF_NULLPTR;
