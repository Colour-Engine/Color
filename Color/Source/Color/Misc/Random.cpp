#include "ColorPCH.h"
#include "Random.h"

FRandomGenerator64 GRandom;

namespace randomalg
{
	uint32 gen32(uint32& iseed)
	{
		iseed ^= iseed << 13;
		iseed ^= iseed >> 17;
		iseed ^= iseed << 5;

		return iseed;
	}

	uint64 gen64(uint64& iseed)
	{
		iseed ^= iseed << 13;
		iseed ^= iseed >> 7;
		iseed ^= iseed << 17;

		return iseed;
	}
}
