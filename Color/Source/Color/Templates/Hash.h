#pragma once

#include "Misc/IntTypes.h"

// To make a type provide hashing support, it must:
//   - Provide a member const method named "Hash", whom returns a "uint_t" value, and takes in a single parameter of type "T" provided to the specialization.
//     The returned value by the "Hash" method must be unique to each unique values of type "T" it can take.
template <typename T>
class THash
{
public:
	THash() = default;
	THash(const THash&) = default;
	~THash() = default;
};

// Hash specialization for integer types are below (int8-int64, uint8-uint64).

template <>
class THash<int8>
{
public:
	uint_t Hash(int8 Value) const
	{
		return (uint_t) Value;
	}
};

template <>
class THash<int16>
{
public:
	uint_t Hash(int16 Value) const
	{
		return (uint_t) Value;
	}
};

template <>
class THash<int32>
{
public:
	uint_t Hash(int32 Value) const
	{
		return (uint_t) Value;
	}
};

template <>
class THash<int64>
{
public:
	uint_t Hash(int64 Value) const
	{
		return (uint_t) Value;
	}
};

template <>
class THash<uint8>
{
public:
	uint_t Hash(uint8 Value) const
	{
		return (uint_t) Value;
	}
};

template <>
class THash<uint16>
{
public:
	uint_t Hash(uint16 Value) const
	{
		return (uint_t) Value;
	}
};

template <>
class THash<uint32>
{
public:
	uint_t Hash(uint32 Value) const
	{
		return (uint_t) Value;
	}
};

template <>
class THash<uint64>
{
public:
	uint_t Hash(uint64 Value) const
	{
		return (uint_t) Value;
	}
};
