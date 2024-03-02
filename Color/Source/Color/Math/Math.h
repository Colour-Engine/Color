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
};
