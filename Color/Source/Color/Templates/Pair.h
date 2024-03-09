#pragma once

#include "Templates/Utility.h"

template <typename T, typename U>
struct TPair
{
	typedef T FirstType;
	typedef U SecondType;

	T First;
	U Second;

	TPair() = default;
	TPair(const TPair&) = default;
	TPair(TPair&&) = default;

	TPair(const T& InFirst, const U& InSecond)
		: First(InFirst), Second(InSecond)
	{
	}

	template <typename T2 = T, typename U2 = U>
	TPair(T2&& InFirst, U2&& InSecond)
		: First(Forward<T2>(InFirst)), Second(Forward<U2>(InSecond))
	{
	}

	template <typename T2, typename U2>
	TPair(const TPair<T2, U2>& Other)
		: First(Other.First), Second(Other.Second)
	{
	}

	template <typename T2, typename U2>
	TPair(TPair<T2, U2>&& Other) noexcept
		: First(Forward<T2>(Other.First)), Second(Forward<U2>(Other.Second))
	{
	}

	TPair& operator=(const TPair& Other)
	{
		First = Other.First;
		Second = Other.Second;

		return *this;
	}

	template <typename T2, typename U2>
	TPair& operator=(const TPair<T2, U2>& Other)
	{
		First = Other.First;
		Second = Other.Second;

		return *this;
	}

	TPair& operator=(TPair&& Other) noexcept
	{
		First = Forward<T>(Other.First);
		Second = Forward<U>(Other.Second);

		return *this;
	}

	template <typename T2, typename U2>
	TPair& operator=(TPair<T2, U2>&& Other) noexcept
	{
		First = Forward<T2>(Other.First);
		Second = Forward<U2>(Other.Second);

		return *this;
	}

	void Swap(TPair& Other)
	{
		::Swap(First, Other.First);
		::Swap(Second, Other.Second);
	}

	bool operator==(const TPair& Other) const
	{
		return First == Other.First && Second == Other.Second;
	}
};
