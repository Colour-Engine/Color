#pragma once

#include "Misc/MiscDefines.h"
#include "Misc/IntTypes.h"

struct FMath
{
	typedef uint8 BaseType;

	static constexpr BaseType BaseOctal       = 8;
	static constexpr BaseType BaseDecimal     = 10;
	static constexpr BaseType BaseHexadecimal = 16;

	template <typename T>
	CL_NODISCARD static constexpr T GetNumDigitsOfInt(T Integer, BaseType Base = BaseDecimal, bool bNegativeCountsAsDigit = true)
	{
		T NumDigits = (bNegativeCountsAsDigit && Base == BaseDecimal && Integer < 0) ? 1 : 0;

		while (Integer)
		{
			Integer /= Base;
			NumDigits++;
		}

		return NumDigits;
	}

	CL_NODISCARD static constexpr double Pow(double A, double B)
	{
		int32 E = (int32) B;

		union
		{
			double D;
			int32 X[2];
		} U = { A };

		U.X[1] = (int32)((B - E) * (U.X[1] - 1072632447) + 1072632447);
		U.X[0] = 0;

		double R = 1.0;
		while (E)
		{
			if (E & 1)
			{
				R *= A;
			}
			A *= A;
			E >>= 1;
		}

		return R * U.D;
	}
};
