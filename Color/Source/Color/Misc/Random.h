#pragma once

#include "Core/Assert.h"

#include "Templates/TypeTraits/IsFloatingPoint.h"
#include "Templates/TypeTraits/MakeUnsigned.h"
#include "Templates/TypeTraits/MakeSigned.h"
#include "Templates/TypeTraits/IsUnsigned.h"
#include "Templates/TypeTraits/IsSigned.h"
#include "Templates/TypeTraits/IsIntegral.h"
#include "Templates/TypeTraits/EnableIf.h"
#include "Templates/TypeTraits/IsSame.h"
#include "Templates/NumericLimits.h"
#include "Templates/Utility.h"

// TODO: In the future, when we have FDateTime, use that instead.
#include <ctime>

// A namespace containing the random algorithms that the FRandomGenerator uses.
namespace randomalg
{
	uint32 gen32(uint32& iseed);
	uint64 gen64(uint64& iseed);
}

template <typename TSeedType = uint64>
class TRandomGenerator
{
public:
	static_assert(VIsSame<TSeedType, uint32> || VIsSame<TSeedType, uint64>, "Only 32-bit and 64-bit unsigned seeds are supported!");
	using SeedType = TSeedType;
public:
	TRandomGenerator() { }
	TRandomGenerator(SeedType InSeed) : Seed(InSeed) { }

	TRandomGenerator(const TRandomGenerator&) = default;
	TRandomGenerator& operator=(const TRandomGenerator&) = default;
	~TRandomGenerator() = default;

	static TRandomGenerator SeededWithTime()
	{
		time_t Time;
		time(&Time);

		return TRandomGenerator(Time);
	}

	// Generates a random integral type number within a range.
	template <typename T, TEnableIf<VIsIntegral<T>, void*> = nullptr>
	T Range(T Min = TNumericLimits<T>::Min(), T Max = TNumericLimits<T>::Max())
	{
		static_assert(!VIsSame<SeedType, uint32> || (VIsSame<SeedType, uint32> && !VIsSame<SeedType, T>), "To generate 64-bit numbers, use a 64-bit seeded random generator!");
		
		if (Min > Max)
		{
			CL_CORE_WARN("FRandomGenerator::Range warning! Min was greater than max, they will be swapped. This is likely an issue within the code, check the generation.");
			::Swap(Min, Max);
		}

		T Range = Max + 1 - Min;

		// Special case where max range is requested.
		if (Range == 0)
		{
			Min++;
		}

		return (Gen() % (Max - Min + 1)) + Min;
	}

	// Random floating-point type number within a range.
	template <typename T, TEnableIf<VIsFloatingPoint<T>, void*> = nullptr>
	T Range(T Min = TNumericLimits<T>::Min(), T Max = TNumericLimits<T>::Max())
	{
		static_assert(!VIsSame<SeedType, uint32> || (VIsSame<SeedType, uint32> && (!VIsSame<double, T> && !VIsSame<long double, T>)), "To generate 64-bit numbers, use a 64-bit seeded random generator!");

		if (Min > Max)
		{
			CL_CORE_WARN("FRandomGenerator::Range warning! Min was greater than max, they will be swapped. This is likely an issue within the code, check the generation.");
			::Swap(Min, Max);
		}

		return Min + ((T)(Gen()) / (T)(TNumericLimits<SeedType>::Max())) * (Max - Min);
	}

	// Generates random bool. Uses uint8 Range random between 0 and 1 to generate false/true.
	bool Bool() { return (bool) Range<uint8>(0, 1); }

	bool BoolWithWeight(float Weight)
	{
		float Scalar = Range<float>(0.0f, 1.0f);
		return Scalar <= Weight;
	}

	int8   Int8()  { return Range<int8>(); }
	int16  Int16() { return Range<int16>(); }
	int32  Int32() { return Range<int32>(); }

	template <TEnableIf<VIsSame<SeedType, uint64>, void*> = nullptr>
	int64  Int64() { return Range<int64>(); }
	
	uint8  UInt8()  { return Range<uint8>(); }
	uint16 UInt16() { return Range<uint16>(); }
	uint32 UInt32() { return Range<uint32>(); }

	template <TEnableIf<VIsSame<SeedType, uint64>, void*> = nullptr>
	uint64 UInt64() { return Range<uint64>(); }

	float       Float()      { return Range<float>(); }

	template <TEnableIf<VIsSame<SeedType, uint64>, void*> = nullptr>
	double      Double()     { return Range<double>(); }

	template <TEnableIf<VIsSame<SeedType, uint64>, void*> = nullptr>
	long double LongDouble() { return Range<long double>(); }

	void SetSeed(SeedType NewSeed) { Seed = NewSeed; }
	SeedType GetSeed() const { return Seed; }
private:
	SeedType Gen()
	{
		if constexpr (VIsSame<SeedType, uint32>)
		{
			return randomalg::gen32(Seed);
		}
		else if constexpr (VIsSame<SeedType, uint64>)
		{
			return randomalg::gen64(Seed);
		}

		unreachable();
		return 0;
	}
private:
	SeedType Seed = 0;
};

using FRandomGenerator32 = TRandomGenerator<uint32>;
using FRandomGenerator64 = TRandomGenerator<uint64>;

// The default global random generator, use this if you don't need to create your own FRandomGenerator.
// Initialized by the FApplication instance when it is constructed in the entry point.
// Uses a 64-bit seed, may not be preferable on 32-bit machines, where you could create your own generator with a 32-bit seed.
extern FRandomGenerator64 GRandom;
