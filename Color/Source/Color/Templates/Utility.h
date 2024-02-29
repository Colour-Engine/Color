#pragma once

#include "TypeTraits/RemoveReference.h"
#include "Misc/IntTypes.h"

template <typename T>
constexpr TRemoveReference<T>&& MoveTemp(T&& Value)
{
	return static_cast<TRemoveReference<T>&&>(Value);
}

template <typename T>
constexpr T&& Forward(TRemoveReference<T>& Value)
{
	return static_cast<T&&>(Value);
}

template <typename T>
constexpr T&& Forward(TRemoveReference<T>&& Value)
{
	return static_cast<T&&>(Value);
}

template <typename T>
constexpr void Swap(T& LHS, T& RHS)
{
	T Temp = MoveTemp(LHS);
	LHS = MoveTemp(RHS);
	RHS = MoveTemp(Temp);
}

template <typename T, uint_t N>
constexpr void Swap(T (&LHS)[N], T (&RHS)[N])
{
	for (uint_t i = 0; i < N; i++)
	{
		Swap(LHS[i], RHS[i]);
	}
}
