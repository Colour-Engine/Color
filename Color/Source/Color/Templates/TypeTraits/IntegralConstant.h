#pragma once

template <typename T, T InValue>
struct TIntegralConstant
{
	typedef T ValueType;
	typedef TIntegralConstant Type;
	static constexpr T Value = InValue;

	operator T() const { return Value; }
	T operator()() const { return Value; }
};

template <bool InBool>
using TBoolConstant = TIntegralConstant<bool, InBool>;

typedef TBoolConstant<false> FFalseType;
typedef TBoolConstant<true>  FTrueType;
