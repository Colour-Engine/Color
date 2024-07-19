#pragma once

#include "Templates/NumericLimits.h"
#include "Utils/StringUtility.h"
#include "Utils/CharUtils.h"

enum class ESearchCase : uint8
{
	// Lowercase & uppercase situations differ (default)
	CaseSensitive = 0,

	// Lowercase & uppercase situations don't differ and are considered the same
	IgnoreCase = 1
};

template <typename T, typename TSizeType = uint_t, TSizeType NPOS_VALUE = TNumericLimits<TSizeType>::Max()>
class StringAlgorithmLib
{
public:
	typedef     T     CharType;
	typedef TSizeType SizeType;
public:
	static constexpr SizeType Npos = NPOS_VALUE;
public:
	static SizeType FindFirstOccurenceOfChar(const T* String, SizeType Length, T Char, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		T CasedChar = CHARCASE(Char, SearchCase);

		for (Position; Position < Length; Position++)
		{
			if (CHARCASE(String[Position], SearchCase) == CasedChar)
			{
				return Position;
			}
		}

		return Npos;
	}

	static SizeType FindFirstOccurenceOfSubstring(const T* String, SizeType StringLength, const T* Substring, SizeType SubstringLength, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		if (SubstringLength > StringLength)
		{
			return false;
		}
		if (SubstringLength == StringLength)
		{
			return INTRNLCMP(String, StringLength, Substring, SubstringLength);
		}

		SizeType InitialIndex = Npos;
		SizeType SubstringIndex = 0;

		for (SizeType i = Position; i < StringLength; i++)
		{
			if (SubstringIndex == SubstringLength - 1)
			{
				return InitialIndex;
			}

			if (CHARCASE(String[i], SearchCase) == CHARCASE(Substring[SubstringIndex], SearchCase))
			{
				if (InitialIndex == Npos)
				{
					InitialIndex = i;
				}

				SubstringIndex++;
			}
			else
			{
				InitialIndex = Npos;
				SubstringIndex = 0;
			}
		}

		if (SubstringIndex == SubstringLength - 1)
		{
			return InitialIndex;
		}

		return Npos;
	}
	
	static TSizeType RfindFirstOccurenceOfChar(const T* String, SizeType Length, T Char, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		T CasedChar = CHARCASE(Char, SearchCase);

		for (Position = (Position == Npos ? Length - 1 : Position); Position != 0; Position--)
		{
			if (CHARCASE(String[Position], SearchCase) == CasedChar)
			{
				return Position;
			}
		}

		return Npos;
	}

	static SizeType RfindFirstOccurenceOfSubstring(const T* String, SizeType StringLength, const T* Substring, SizeType SubstringLength, SizeType Position = Npos, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		if (SubstringLength > StringLength)
		{
			return false;
		}
		if (SubstringLength == StringLength)
		{
			return INTRNLCMP(String, StringLength, Substring, SubstringLength, SearchCase);
		}

		const SizeType DefSubstrIdx = SubstringLength - 1;
		SizeType SubstringIndex = DefSubstrIdx;

		for (SizeType i = (Position == Npos ? StringLength - 1 : Position); i != 0; i--)
		{
			if (CHARCASE(String[i], SearchCase) == CHARCASE(Substring[SubstringIndex], SearchCase))
			{
				if (SubstringIndex == 0)
				{
					return i;
				}

				SubstringIndex--;
			}
			else
			{
				SubstringIndex = DefSubstrIdx;
			}
		}

		if (SubstringIndex == 0)
		{
			return 0;
		}

		return Npos;
	}

	static SizeType FindFirstOfCharOccurenceInCharset(const T* String, SizeType StringLength, const T* Charset, SizeType CharsetLength, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		for (Position; Position < StringLength; Position++)
		{
			for (SizeType i = 0; i < CharsetLength; i++)
			{
				if (CHARCASE(String[Position], SearchCase) == CHARCASE(Charset[i], SearchCase))
				{
					return Position;
				}
			}
		}

		return Npos;
	}

	static SizeType FindFirstCharNotOfOccurrence(const T* String, SizeType Length, T Char, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		T CasedChar = CHARCASE(Char, SearchCase);

		for (Position; Position < Length; Position++)
		{
			if (CHARCASE(String[Position], SearchCase) != CasedChar)
			{
				return Position;
			}
		}

		return Npos;
	}

	static SizeType FindFirstCharNotOfOccurrenceInCharset(const T* String, SizeType Length, const T* Charset, SizeType CharsetLength, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		for (Position; Position < Length; Position++)
		{
			bool bMatched = false;

			for (SizeType i = 0; i < CharsetLength; i++)
			{
				if (CHARCASE(String[Position], SearchCase) == CHARCASE(Charset[i], SearchCase))
				{
					bMatched = true;
					break;
				}
			}

			if (!bMatched)
			{
				return Position;
			}
		}

		return Npos;
	}

	static SizeType FindLastOfCharOccurrenceInCharset(const T* String, SizeType Length, const T* Charset, SizeType CharsetLength, SizeType Position = Npos, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		for (Position = (Position == Npos ? Length - 1 : Position); Position != 0; Position--)
		{
			for (SizeType i = 0; i < CharsetLength; i++)
			{
				if (CHARCASE(String[Position], SearchCase) == CHARCASE(Charset[i], SearchCase))
				{
					return Position;
				}
			}
		}

		return Npos;
	}

	static SizeType FindLastCharNotOfOccurrence(const T* String, SizeType Length, T Char, SizeType Position = Npos, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		T CasedChar = CHARCASE(Char, SearchCase);

		for (Position = (Position == Npos ? Length - 1 : Position); Position != 0; Position--)
		{
			if (CHARCASE(String[Position], SearchCase) != CasedChar)
			{
				return Position;
			}
		}

		return Npos;
	}

	static SizeType FindLastNotOfCharOccurrenceInCharset(const T* String, SizeType Length, const T* Charset, SizeType CharsetLength, SizeType Position = Npos, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		for (Position = (Position == Npos ? Length - 1 : Position); Position != 0; Position--)
		{
			bool bMatched = false;

			for (SizeType i = 0; i < CharsetLength; i++)
			{
				if (CHARCASE(String[Position], SearchCase) == CHARCASE(Charset[i], SearchCase))
				{
					bMatched = true;
					break;
				}
			}

			if (!bMatched)
			{
				return Position;
			}
		}

		return Npos;
	}

	static bool StartsWithChar(const T* String, SizeType Length, T Char, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		return Length > 0 ? CHARCASE(*String, SearchCase) == CHARCASE(Char, SearchCase) : false;
	}

	static bool StarsWith(const T* String, SizeType StringLength, const T* Substring, SizeType SubstringLength, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		if (SubstringLength > StringLength)
		{
			return false;
		}
		if (SubstringLength == StringLength)
		{
			return INTRNLCMP(String, StringLength, Substring, SubstringLength, SearchCase);
		}

		for (SizeType i = 0; i < StringLength; i++)
		{
			if (CHARCASE(String[i], SearchCase) != CHARCASE(Substring[i], SearchCase))
			{
				return false;
			}

			if (i == SubstringLength - 1)
			{
				return true;
			}
		}

		return false;
	}

	static bool EndsWithChar(const T* String, SizeType Length, T Char, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		return Length > 0 ? CHARCASE(String[Length - 1], SearchCase) == CHARCASE(Char, SearchCase) : false;
	}

	static bool EndsWith(const T* String, SizeType StringLength, const T* Substring, SizeType SubstringLength, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		if (SubstringLength > StringLength)
		{
			return false;
		}
		if (SubstringLength == StringLength)
		{
			return *this == Substring;
		}

		SizeType SubstringIndex = 0;
		for (SizeType i = StringLength - SubstringLength - 1; i < StringLength; i++)
		{
			if (SubstringIndex == SubstringLength - 1)
			{
				return true;
			}

			if (CHARCASE(String[i], SearchCase) == CHARCASE(Substring[SubstringIndex], SearchCase))
			{
				SubstringIndex++;
			}
			else
			{
				SubstringIndex = 0;
			}
		}

		return SubstringIndex == SubstringLength - 1;
	}

	static bool ContainsChar(const T* String, SizeType Length, T Char, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		T CasedChar = CHARCASE(Char, SearchCase);

		for (SizeType i = 0; i < Length; i++)
		{
			if (CHARCASE(String[i], SearchCase) == CasedChar)
			{
				return true;
			}
		}

		return false;
	}

	static bool ContainsAnyCharInCharset(const T* String, SizeType StringLength, const T* Charset, SizeType CharsetLength, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		for (SizeType i = 0; i < StringLength; i++)
		{
			for (SizeType j = 0; j < CharsetLength; j++)
			{
				if (CHARCASE(String[i], SearchCase) == CHARCASE(Charset[i], SearchCase))
				{
					return true;
				}
			}
		}

		return false;
	}

	static bool ContainsSubstring(const T* String, SizeType StringLength, const T* Substring, SizeType SubstringLength, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		if (SubstringLength > StringLength)
		{
			return false;
		}
		if (SubstringLength == StringLength)
		{
			return INTRNLCMP(String, StringLength, Substring, SubstringLength, SearchCase);
		}

		SizeType SubstringIndex = 0;
		for (SizeType i = 0; i < StringLength; i++)
		{
			if (SubstringIndex == SubstringLength - 1)
			{
				return true;
			}

			if (CHARCASE(String[i], SearchCase) == CHARCASE(Substring[SubstringIndex], SearchCase))
			{
				SubstringIndex++;
			}
			else
			{
				SubstringIndex = 0;
			}
		}

		return SubstringIndex == SubstringLength - 1;
	}

	// Not lexical!
	static bool EqualityCompare(const T* Left, SizeType LeftSz, const T* Right, SizeType RightSz, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		return INTRNLCMP(Left, LeftSz, Right, RightSz, SearchCase);
	}
private:
	static T CHARCASE(T Char, ESearchCase SearchCase)
	{
		switch (SearchCase)
		{
		case ESearchCase::CaseSensitive: return Char;
		case ESearchCase::IgnoreCase:    return TCharUtils<T>::ToLower(Char);
		}

		return (T) 0x0;
	}

	static bool INTRNLCMP(const T* Left, SizeType LeftSz, const T* Right, SizeType RightSz, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		// If true, it's guaranteed that they have the same amount of memory occupation, which is made use of after the if statement.
		if (LeftSz != RightSz)
		{
			return false;
		}

		const uint8* RawLeft = (const uint8*) Left;
		const uint8* RawRight = (const uint8*) Right;

		while (LeftSz--)
		{
			if (CHARCASE(*RawLeft++, SearchCase) != CHARCASE(*RawRight++, SearchCase))
			{
				return false;
			}
		}

		return CHARCASE(*RawLeft, SearchCase) == CHARCASE(*RawRight, SearchCase);
	}
};
