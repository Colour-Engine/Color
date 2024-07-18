#pragma once

#include "Allocators/DefaultAllocator.h"
#include "Templates/TypeTraits/IsUnsigned.h" // Used in FromInt
#include "Templates/TypeTraits/IsSigned.h" // Used in ToInteger
#include "Templates/NumericLimits.h"
#include "Templates/Hash.h"

#include "Core/Memory.h"
#include "Misc/ExitCode.h"
#include "Misc/NoInit.h"
#include "Math/Math.h"

#include "Misc/RuntimeErrorManager.h"
#include "Containers/Iterator.h"
#include "Utils/StringUtility.h"

#include <initializer_list>
#include <stdarg.h>
#include <map>

#ifndef CL_SHIPPING
	#define CL_TString_AttentionRequired(InExitCode) CL_PLATFORM_DEBUGBREAK()
#else
	#define CL_TString_AttentionRequired(InExitCode) FRuntimeErrorManager::RtFatal("TString internal error (exitcode %d)", (int32) ExitCode::InExitCode)
#endif

template <typename T, typename TSizeType = uint_t, typename TAllocatorType = TDefaultAllocator<T>>
class TString
{
public:
	using CharType      = T;
	using SizeType      = TSizeType;
	using AllocatorType = TAllocatorType;
	using StringUtility = TStringUtility<T>;
	using Iterator      = TIndexedContainerIterator<TString, T, TSizeType>;
	using ConstIterator = TIndexedContainerIterator<const TString, const T, TSizeType>;
public:
	static constexpr SizeType Npos = TNumericLimits<SizeType>::Max();
	static constexpr SizeType BlockSize = 30;
public:
	TString(TYPE_OF_NULLPTR) = delete;
	TString& operator=(TYPE_OF_NULLPTR) = delete;
	TString(ENoInit) { }

	TString(const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Allocate(BlockSize + 1, true);
		InsertNullTerminator();
	}

	TString(const TString& Other)
	{
		Copy(Other);
	}

	TString(const TString& Other, const AllocatorType& Allocator)
		: Allocator(Allocator)
	{
		Copy(Other, false);
	}

	TString(TString&& Other) noexcept
	{
		Move(MoveTemp(Other));
	}

	TString(TString&& Other, const AllocatorType& Allocator)
		: Allocator(Allocator)
	{
		Move(MoveTemp(Other), false);
	}

	TString(std::initializer_list<T> InitList, const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Copy(InitList);
	}

	TString(const T* String, SizeType Size, const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Copy(String, Size);
	}

	TString(const T* String, const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Copy(String);
	}

	TString(T Char, SizeType Count, const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Allocate(Count);
		Size = Count;

		SetRange(Char, 0, Count);
		InsertNullTerminator();
	}

	TString(T Char, const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Allocate(2);
		Size = 1;

		Data[0] = Char;
		Data[1] = '\0';
	}

	~TString()
	{
		Destruct();
	}

	TString& operator=(const TString& String)
	{
		Copy(String);
		return *this;
	}

	TString& operator=(TString&& String) noexcept
	{
		Move(MoveTemp(String));
		return *this;
	}

	TString& operator=(const T* String)
	{
		Copy(String);
		return *this;
	}

	TString& operator=(T Char)
	{
		Destruct();
		Allocate(2);

		Data[0] = Char;
		Data[1] = '\0';
		Size = 1;

		return *this;
	}

	TString& operator=(std::initializer_list<T> InitList)
	{
		Copy(InitList);
		return *this;
	}

	TString& operator+=(T Char)
	{
		Push(Char);
		return *this;
	}

	TString& operator+=(const T* String)
	{
		Append(String);
		return *this;
	}

	TString& operator+=(const TString& Other)
	{
		Append(Other);
		return *this;
	}

	TString& operator+=(std::initializer_list<T> InitList)
	{
		Append(InitList);
		return *this;
	}

	TString operator+(T Char) const
	{
		return Appended(Char);
	}

	TString operator+(const T* String) const
	{
		return Appended(String);
	}

	TString operator+(const TString& Other) const
	{
		return Appended(Other);
	}

	TString operator+(std::initializer_list<T> InitList) const
	{
		return Appended(InitList);
	}

	TString operator~() const
	{
		return Reversed();
	}

	TString Appended(T Char) const
	{
		TString Result = *this;
		Result.Push(Char);

		return Result;
	}

	TString Appended(const T* String, SizeType Length) const
	{
		TString Result(NO_INIT);
		Result.Allocate(Size + Length);

		FMemory::Copy(Result.Data, Data, Size);
		FMemory::Copy(Result.Data + Size, String, Length);

		Result.Size = Size + Length;
		Result.InsertNullTerminator();

		return Result;
	}

	TString Appended(const T* String) const
	{
		return Appended(String, (SizeType) StringUtility::Len(String));
	}

	TString Appended(const TString& Other) const
	{
		return Appended(Other.Data, Other.Size);
	}

	TString Appended(std::initializer_list<T> InitList) const
	{
		return Appended(InitList.begin(), (SizeType) InitList.size());
	}

	// Appends a char N amount of times.
	void Append(SizeType Count, T Char)
	{
		Reserve(Size + Count);

		while (Count--)
		{
			Data[Size++] = Char;
		}

		InsertNullTerminator();
	}

	void Append(T Char)
	{
		Push(Char);
	}

	void Append(const T* String, SizeType Count)
	{
		Reserve(Size + Count);
		FMemory::Copy(Data + Size, String, Count);
		Size += Count;
		InsertNullTerminator();
	}

	void Append(const T* String)
	{
		Append(String, (SizeType) StringUtility::Len(String));
	}

	void Append(const TString& Other)
	{
		Append(Other.Data, Other.Size);
	}

	void Append(std::initializer_list<T> InitList)
	{
		Reserve(Size + (SizeType) InitList.size());

		for (T Char : InitList)
		{
			Push(Char);
		}

		InsertNullTerminator();
	}

	void Push(T Char)
	{
		GrowIfNecessary();
		Data[Size++] = Char;
		InsertNullTerminator();
	}

	void PushFront(T Char)
	{
		GrowIfNecessary();

		Size++;
		for (SizeType i = Size - 1; i > 0; i--)
		{
			Data[i] = Data[i - 1];
		}
		Data[0] = Char;

		InsertNullTerminator();
	}

	// Concates all given strings at once, the most performant way for concating multiple strings at once.
	// Strings must match to Sizes! i.e. length of the string at Strings[0] must be equal to Sizes[0] and so on and so forth.
	static TString BulkConcate(const T** Strings, uint_t* Sizes, uint_t NumStrings)
	{
		uint_t TotalSize = 0;

		for (uint_t i = 0; i < NumStrings; i++)
		{
			TotalSize += Sizes[i];
		}

		TString Result(NO_INIT);
		Result.Allocate(TotalSize);

		uint_t Position = 0;
		for (uint_t i = 0; i < NumStrings; i++)
		{
			FMemory::Copy(Result.Data + Position, Strings[i], Sizes[i]);
			Position += Sizes[i];
		}
		
		Result.Size = TotalSize;
		Result.InsertNullTerminator();

		return Result;
	}

	bool RemoveAt(SizeType Index)
	{
		if (!IsValidIndex(Index))
		{
			return false;
		}

		Data[Index] = 0;
		for (SizeType i = Index; i < Size - 1; i++)
		{
			Data[i] = Data[i + 1];
		}
		Size--;

		return true;
	}

	void Pop()
	{
		if (Size > 0)
		{
			Data[--Size] = 0;
		}
	}

	void PopFront()
	{
		if (Size > 0)
		{
			// This operation will pull the null terminator back by one char, since we don't change Size yet.
			for (SizeType i = 0; i < Size; i++)
			{
				Data[i] = Data[i + 1];
			}

			Size--;
		}
	}

	void Clear()
	{
		for (SizeType i = 0; i < Size; i++)
		{
			Data[i] = 0;
		}

		Size = 0;
	}

	void Resize(SizeType NewSize, T CharToInsert)
	{
		if (Size > NewSize)
		{
			SizeType PrevSize = Size;
			Size = NewSize;

			for (SizeType i = Size - 1; i < PrevSize; i++)
			{
				Data[i] = 0;
			}
		}
		else if (Size < NewSize)
		{
			SizeType Diff = NewSize - Size;

			if (NewSize > Capacity)
			{
				ReAllocate(NewSize);
			}

			while (Diff--)
			{
				Data[Size++] = CharToInsert;
			}
		}

		InsertNullTerminator();
	}

	void Resize(SizeType NewSize)
	{
		Resize(NewSize, (T) 0);
	}

	void Reserve(SizeType Cap)
	{
		if (Capacity <= Cap)
		{
			ReAllocate(Cap);
		}
	}

	void Shrink()
	{
		if (Capacity > Size)
		{
			ReAllocate(Size, true);
		}
	}

	// Format Specifiers:
	//   %% - Writes '%'.
	//   %s - String (char*).
	//   %d OR %i - Signed integer (max 32-bit).
	//   %u - Unsigned integer (max 32-bit).
	//   %l - Signed integer (64-bit specific).
	//   %e - Unsigned integer (64-bit specific).
	//   %c - Character.
	//   %b - Boolean.
	//   %f - Float or Double.
	static TString Format(const T* Fmt, va_list Args)
	{
		TString Result;

		while (*Fmt)
		{
			T Char = *Fmt++;

			if (Char == '%')
			{
				if (!*Fmt)
				{
					Result = "[[String format failure!]] Format indicator found but the string was terminated before the format specifier was found!";
					return Result;
				}

				T Specifier = *Fmt++;
				switch (Specifier)
				{
				case '%':
				{
					Result += '%';
					break;
				}
				case 's':
				{
					T* String = va_arg(Args, T*);
					Result += String;
					break;
				}
				case 'i':
				case 'd':
				{
					int32 Integer = va_arg(Args, int32);
					Result += TString::FromInt(Integer);
					break;
				}
				case 'u':
				{
					uint32 Integer = va_arg(Args, uint32);
					Result += TString::FromInt(Integer);
					break;
				}
				case 'l':
				{
					int64 Integer = va_arg(Args, int64);
					Result += TString::FromInt(Integer);
					break;
				}
				case 'e':
				{
					uint64 Integer = va_arg(Args, uint64);
					Result += TString::FromInt(Integer);
					break;
				}
				case 'c':
				{
					int32 Char = va_arg(Args, int32);
					Result += (T) Char;
					break;
				}
				case 'b':
				{
					int32 Bool = va_arg(Args, int32);
					Result += Bool ? "true" : "false";
					break;
				}
				case 'f':
				{
					double FloatOrDouble = va_arg(Args, double);
					Result += TString::FromDouble(FloatOrDouble);
					break;
				}
				default:
				{
					Result = TString("[[String format failure!]] Specifier '") + Specifier + "' is not a valid specifier!";
					return Result;
				}
				}
			}
			else
			{
				Result += Char;
			}
		}

		return Result;
	}

	// Format Specifiers:
	//   %% - Writes '%'.
	//   %s - String (char*).
	//   %d OR %i - Signed integer (max 32-bit).
	//   %u - Unsigned integer (max 32-bit).
	//   %l - Signed integer (64-bit specific).
	//   %e - Unsigned integer (64-bit specific).
	//   %c - Character.
	//   %b - Boolean.
	//   %f - Float or Double.
	static TString Format(const T* Fmt, ...)
	{
		va_list Args;
		va_start(Args, Fmt);

		TString Result = Format(Fmt, Args);
		va_end(Args);

		return Result;
	}

	template <typename TInt>
	static TString FromInt(TInt Integer, FMath::BaseType Base = FMath::BaseDecimal)
	{
		if (Integer == 0)
		{
			return "0";
		}

		SizeType Length = FMath::GetNumDigitsOfInt(Integer);
		TString Result(NO_INIT);
		Result.Allocate(Length);
		Result.Size = Length;

		bool bNegative = false;
		if constexpr (!VIsUnsigned<TInt>) // This if check is here to shut compiler warnings up. Also, unsigned integers can never be negative, so the cheks below are useless.
		{
			if (Integer < 0 && Base == FMath::BaseDecimal)
			{
				bNegative = true;
				Integer = -Integer;
			}
		}

		SizeType i = 0;
		while (Integer)
		{
			T Remainder = Integer % Base;
			Result[i++] = (Remainder > 9) ? (Remainder - 10) + 'a' : Remainder + '0';
			Integer /= Base;
		}

		if (bNegative)
		{
			Result[i] = '-';
		}

		Result.InsertNullTerminator();
		Result.Reverse();

		return Result;
	}

	static TString FromDouble(double Double)
	{
		char Buffer[1000];
		sprintf(Buffer, "%lf", Double);
		return Buffer;
	}

	static TString FromFloat(float Float)
	{
		return FromDouble((double) Float);
	}

	template <typename TInt = int32>
	static TInt ToInteger(const T* String, SizeType Length)
	{
		if (Length == 0)
		{
			return TInt(0);
		}
		
		TInt Parsed = 0;
		for (SizeType i = 0; i < Length; i++)
		{
			Parsed = (Parsed << 3) + (Parsed << 1) + (*String++) - '0';
		}

		if constexpr (VIsSigned<TInt>)
		{
			if (String[0] == '-')
			{
				Parsed = -Parsed;
			}
		}

		return Parsed;
	}

	template <typename TInt = int32>
	static TInt ToInteger(const T* String)
	{
		return ToInteger<TInt>(String, (SizeType) StringUtility::Len(String));
	}

	template <typename TInt = int32>
	TInt ToInteger() const
	{
		return TString::ToInteger<TInt>(Data, Size);
	}

	static double ToDouble(const T* String, SizeType Length)
	{
	//	double Rez = 0, Fact = 1;
	//
	//	if (*String == '-')
	//	{
	//		String++;
	//		Fact = -1;
	//	};
	//
	//	for (int Flag = 0; *String; String++)
	//	{
	//		if (*String == '.')
	//		{
	//			Flag = 1;
	//			continue;
	//		};
	//
	//		int Digit = *String - '0';
	//		if (Digit >= 0 && Digit <= 9)
	//		{
	//			if (Flag)
	//			{
	//				Fact /= 10.0f;
	//			}
	//
	//			Rez = Rez * 10.0 + (double) Digit;
	//		};
	//	};
	//
	//	return Rez * Fact;

		char* EndPos = nullptr;
		return strtod(String, &EndPos);
	}

	static double ToDouble(const T* String)
	{
		return ToDouble(String, (SizeType) StringUtility::Len(String));
	}

	static float ToFloat(const T* String, SizeType Length)
	{
		return (float) ToDouble(String, Length);
	}

	static float ToFloat(const T* String)
	{
		return (float) ToDouble(String);
	}

	double ToDouble() const
	{
		return TString::ToDouble(Data, Size);
	}

	float ToFloat() const
	{
		return TString::ToFloat(Data, Size);
	}

	void Reverse()
	{
		SizeType MaxIndex = Size-1;

		for (SizeType i = 0; i < Size / 2; i++)
		{
			SizeType Otherside = MaxIndex - i;
			char Tmp = Data[i];

			Data[i] = Data[Otherside];
			Data[Otherside] = Tmp;
		}
	}

	TString Reversed() const
	{
		TString Result = *this;
		Result.Reverse();

		return Result;
	}

	void Replace(T From, T To)
	{
		for (SizeType i = 0; i < Size; i++)
		{
			if (Data[i] == From)
			{
				Data[i] = To;
			}
		}
	}

	/**
	 * Sets a certain amount of characters, starting at the given position index to the given character.
	 *
	 * @param Char The character to set the characters within the range to.
	 * @param Position The index at which the changes will start happening.
	 * @param Count Amount of characters to change. Leave as Npos if every character in the array from the starting index is going to be changed.
	 */
	void SetRange(T Char, SizeType Position = 0, SizeType Count = Npos)
	{
		SizeType EndIndex = (Count != Npos ? Count : Size - Position) + Position;
		
		if (!IsValidIndex(EndIndex))
		{
			// EndIndex is out of bounds!
			CL_TString_AttentionRequired(StrIndexOutOfBounds);
			return;
		}

		for (Position; Position < EndIndex; Position++)
		{
			Data[Position] = Char;
		}
	}

	// Sets every character within the string to the given value.
	// Basically, it just calls SetRange(Value, 0, Npos).
	void Fill(T Char)
	{
		SetRange(Char);
	}

	void Swap(TString& Other)
	{
		::Swap(Data, Other.Data);
		::Swap(Size, Other.Size);
		::Swap(Capacity, Other.Capacity);
		::Swap(Allocator, Other.Allocator);
	}

	SizeType Find(T Char, SizeType Position = 0) const
	{
		for (Position; Position < Size; Position++)
		{
			if (Data[Position] == Char)
			{
				return Position;
			}
		}

		return Npos;
	}

	SizeType Find(const T* Substring, SizeType Position, SizeType Length) const
	{
		if (Length > Size)
		{
			return false;
		}
		if (Length == Size)
		{
			return *this == Substring;
		}

		SizeType InitialIndex = Npos;
		SizeType SubstringIndex = 0;

		for (SizeType i = Position; i < Size; i++)
		{
			if (SubstringIndex == Length - 1)
			{
				return InitialIndex;
			}

			if (Data[i] == Substring[SubstringIndex])
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

		if (SubstringIndex == Length - 1)
		{
			return InitialIndex;
		}

		return Npos;
	}

	SizeType Find(const T* Substring, SizeType Position = 0) const
	{
		return Find(Substring, Position, (SizeType) StringUtility::Len(Substring));
	}

	SizeType Find(const TString& Substring, SizeType Position = 0) const
	{
		return Find(Substring.Data, Position, Substring.Size);
	}

	SizeType Rfind(T Char, SizeType Position = Npos) const
	{
		for (Position = (Position == Npos ? Size - 1 : Position); Position != 0; Position--)
		{
			if (Data[Position] == Char)
			{
				return Position;
			}
		}

		return Npos;
	}

	SizeType Rfind(const T* Substring, SizeType Position, SizeType Length) const
	{
		if (Length > Size)
		{
			return false;
		}
		if (Length == Size)
		{
			return *this == Substring;
		}

		const SizeType DefSubstrIdx = Length - 1;
		SizeType SubstringIndex = DefSubstrIdx;

		for (SizeType i = (Position == Npos ? Size - 1 : Position); i != 0; i--)
		{
			if (Data[i] == Substring[SubstringIndex])
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

	SizeType Rfind(const T* Substring, SizeType Position = Npos) const
	{
		return Rfind(Substring, Position, (SizeType) StringUtility::Len(Substring));
	}

	SizeType Rfind(const TString& Substring, SizeType Position = Npos) const
	{
		return Rfind(Substring.Data, Position, Substring.Size);
	}

	SizeType FindFirstOf(T Char, SizeType Position = 0) const
	{
		return Find(Char, Position);
	}

	SizeType FindFirstOf(const T* Charset, SizeType Position, SizeType Length) const
	{
		for (Position; Position < Size; Position++)
		{
			for (SizeType i = 0; i < Length; i++)
			{
				if (Data[Position] == Charset[i])
				{
					return Position;
				}
			}
		}

		return Npos;
	}

	SizeType FindFirstOf(const T* Charset, SizeType Position = 0) const
	{
		return FindFirstOf(Charset, Position, (SizeType) StringUtility::Len(Charset));
	}

	SizeType FindFirstOf(const TString& Charset, SizeType Position = 0) const
	{
		return FindFirstOf(Charset.Data, Position, Charset.Size);
	}

	SizeType FindFirstNotOf(T Char, SizeType Position = 0) const
	{
		for (Position; Position < Size; Position++)
		{
			if (Data[Position] != Char)
			{
				return Position;
			}
		}

		return Npos;
	}

	SizeType FindFirstNotOf(const T* Charset, SizeType Position, SizeType Length) const
	{
		for (Position; Position < Size; Position++)
		{
			bool bMatched = false;

			for (SizeType i = 0; i < Length; i++)
			{
				if (Data[Position] == Charset[i])
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

	SizeType FindFirstNotOf(const T* Charset, SizeType Position = 0) const
	{
		return FindFirstNotOf(Charset, Position, (SizeType) StringUtility::Len(Charset));
	}

	SizeType FindFirstNotOf(const TString& Charset, SizeType Position = 0) const
	{
		return FindFirstNotOf(Charset.Data, Position, Charset.Size);
	}

	SizeType FindLastOf(T Char, SizeType Position = Npos) const
	{
		return Rfind(Char, Position);
	}

	SizeType FindLastOf(const T* Charset, SizeType Position, SizeType Length) const
	{
		for (Position = (Position == Npos ? Size - 1 : Position); Position != 0; Position--)
		{
			for (SizeType i = 0; i < Length; i++)
			{
				if (Data[Position] == Charset[i])
				{
					return Position;
				}
			}
		}

		return Npos;
	}

	SizeType FindLastOf(const T* Charset, SizeType Position = Npos) const
	{
		return FindLastOf(Charset, Position, (SizeType) StringUtility::Len(Charset));
	}

	SizeType FindLastOf(const TString& Charset, SizeType Position = Npos) const
	{
		return FindLastOf(Charset.Data, Position, Charset.Size);
	}

	SizeType FindLastNotOf(T Char, SizeType Position = Npos) const
	{
		for (Position = (Position == Npos ? Size - 1 : Position); Position != 0; Position--)
		{
			if (Data[Position] != Char)
			{
				return Position;
			}
		}

		return Npos;
	}

	SizeType FindLastNotOf(const T* Charset, SizeType Position, SizeType Length) const
	{
		for (Position = (Position == Npos ? Size - 1 : Position); Position != 0; Position--)
		{
			bool bMatched = false;

			for (SizeType i = 0; i < Length; i++)
			{
				if (Data[Position] == Charset[i])
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

	SizeType FindLastNotOf(const T* Charset, SizeType Position = Npos) const
	{
		return FindLastNotOf(Charset, Position, (SizeType) StringUtility::Len(Charset));
	}

	SizeType FindLastNotOf(const TString& Charset, SizeType Position = Npos) const
	{
		return FindLastNotOf(Charset.Data, Position, Charset.Size);
	}

	TString Sub(SizeType Position = 0, SizeType Count = Npos) const
	{
		return TString(Data + Position, Count == Npos ? Size - Position : Count);
	}

	bool StartsWith(T Char) const
	{
		return *Data == Char;
	}

	bool StartsWith(const T* Substring, SizeType Length) const
	{
		if (Length > Size)
		{
			return false;
		}
		if (Length == Size)
		{
			return *this == Substring;
		}

		for (SizeType i = 0; i < Size; i++)
		{
			if (Data[i] != Substring[i])
			{
				return false;
			}

			if (i == Length - 1)
			{
				return true;
			}
		}

		return false;
	}

	bool StartsWith(const T* Substring) const
	{
		return StartsWith(Substring, (SizeType) StringUtility::Len(Substring));
	}

	bool StartsWith(const TString& Substring) const
	{
		return StartsWith(Substring.Data, Substring.Size);
	}

	bool EndsWith(T Char) const
	{
		return Back() == Char;
	}

	bool EndsWith(const T* Substring, SizeType Length) const
	{
		if (Length > Size)
		{
			return false;
		}
		if (Length == Size)
		{
			return *this == Substring;
		}

		SizeType SubstringIndex = 0;
		for (SizeType i = Size - Length - 1; i < Size; i++)
		{
			if (SubstringIndex == Length - 1)
			{
				return true;
			}

			if (Data[i] == Substring[SubstringIndex])
			{
				SubstringIndex++;
			}
			else
			{
				SubstringIndex = 0;
			}
		}

		return SubstringIndex == Length - 1;
	}

	bool EndsWith(const T* Substring) const
	{
		return EndsWith(Substring, (SizeType) StringUtility::Len(Substring));
	}

	bool EndsWith(const TString& Substring) const
	{
		return EndsWith(Substring.Data, Substring.Size);
	}

	bool ContainsAnyOf(const T* Charset, SizeType Length) const
	{
		for (SizeType i = 0; i < Size; i++)
		{
			for (SizeType j = 0; j < Length; j++)
			{
				if (Data[i] == Charset[i])
				{
					return true;
				}
			}
		}

		return false;
	}

	bool ContainsAnyOf(const T* Charset) const
	{
		return ContainsAnyOf(Charset, (SizeType) StringUtility::Len(Charset));
	}

	bool ContainsAnyOf(const TString& Charset) const
	{
		return ContainsAnyOf(Charset, Charset.Size);
	}

	bool Contains(const T* Substring, SizeType Length) const
	{
		if (Length > Size)
		{
			return false;
		}
		if (Length == Size)
		{
			return *this == Substring;
		}

		SizeType SubstringIndex = 0;
		for (SizeType i = 0; i < Size; i++)
		{
			if (SubstringIndex == Length - 1)
			{
				return true;
			}

			if (Data[i] == Substring[SubstringIndex])
			{
				SubstringIndex++;
			}
			else
			{
				SubstringIndex = 0;
			}
		}

		return SubstringIndex == Length - 1;
	}

	bool Contains(const T* Substring) const
	{
		return Contains(Substring, (SizeType) StringUtility::Len(Substring));
	}

	bool Contains(const TString& Substring) const
	{
		return Contains(Substring.Data, Substring.Size);
	}

	int32 Compare(const T* String) const
	{
		return StringUtility::Cmp(Data, String);
	}

	int32 Compare(const TString& Other) const
	{
		return Compare(Other.Data);
	}

	bool IsValidIndex(SizeType Index) const
	{
		return Index >= 0 && Index < Size;
	}

	Iterator CreateIterator()
	{
		return Iterator(*this);
	}

	ConstIterator CreateConstIterator() const
	{
		return ConstIterator(*this);
	}

	Iterator begin() { return CreateIterator(); }
	Iterator end() { Iterator It = CreateIterator(); It.SetToEnd(); return It; }

	ConstIterator begin() const { return CreateConstIterator(); }
	ConstIterator end() const { ConstIterator It = CreateConstIterator(); It.SetToEnd(); return It; }

	ConstIterator cbegin() const { return begin(); }
	ConstIterator cend() const { return end(); }

	const T* Get() const { return Data; }
	T* Get() { return Data; }

	const T* operator*() const { return Data; }
	T* operator*() { return Data; }

	const AllocatorType& GetAllocator() const { return Allocator; }
	AllocatorType& GetAllocator() { return Allocator; }

	SizeType GetAllocatedSize() const { return Capacity * sizeof(T); }
	SizeType MaxSize() const { return TNumericLimits<SizeType>::Max(); }
	SizeType GetMaxIndex() const { return Size > 0 ? Size - 1 : 0; }

	// For container support. Prefer using Len() when working with strings.
	SizeType Num() const { return Size; }
	
	SizeType Len() const { return Size; }
	SizeType Max() const { return Capacity; }
	CL_NODISCARD bool IsEmpty() const { return Size == 0; }

	const T& Front() const { return *Data; }
	T& Front() { return *Data; }

	const T& First() const { return Front(); }
	T& First() { return Front(); }

	const T& Back() const { return *(Data+Size-1); }
	T& Back() { return *(Data+Size-1); }

	const T& Last() const { return Back(); }
	T& Last() { return Back(); }

	const T& At(SizeType Index) const
	{
		if (!IsValidIndex(Index))
		{
			// Index out of bounds!
			CL_TString_AttentionRequired(StrIndexOutOfBounds);
		}

		return Data[Index];
	}

	T& At(SizeType Index)
	{
		if (!IsValidIndex(Index))
		{
			// Index out of bounds!
			CL_TString_AttentionRequired(StrIndexOutOfBounds);
		}

		return Data[Index];
	}

	const T& operator[](SizeType Index) const { return Data[Index]; }
	T& operator[](SizeType Index) { return Data[Index]; }

	bool operator==(const T* String) const
	{
		return Compare(String) == 0;
	}

	bool operator==(const TString& Other) const
	{
		return Compare(Other) == 0;
	}
private:
	void Allocate(SizeType Cap, bool bStrictlyUseGivenSize = false)
	{
		Size = 0;
		Capacity = bStrictlyUseGivenSize ? Cap : GetCapacityForSize(Cap);
		Data = Allocator.Allocate(Capacity);
	}

	void ReAllocate(SizeType Cap, bool bStrictlyUseGivenSize = false)
	{
		SizeType NewCapacity = bStrictlyUseGivenSize ? Cap : GetCapacityForSize(Cap);
		T* NewData = Allocator.Allocate(NewCapacity);

		// We also copy the null terminator by copying Size+1 amount of bytes, so no need to call InsertNullTerminator().
		FMemory::Copy(NewData, Data, Size + 1);

		Allocator.DeAllocate(Data, Capacity);
		Data = NewData;
		Capacity = NewCapacity;
	}

	void Grow()
	{
		// +1 triggers new block step.
		ReAllocate(Capacity+1);
	}

	void GrowIfNecessary()
	{
		if (Size == Capacity - 1)
		{
			Grow();
		}
	}

	void Copy(const T* Pointer, SizeType Size)
	{
		Destruct();

		if (Size != 0)
		{
			Allocate(Size);
			this->Size = Size;

			for (SizeType i = 0; i < Size; i++)
			{
				Data[i] = Pointer[i];
			}

			InsertNullTerminator();
		}
		else
		{
			Allocate(BlockSize + 1, true);
			Size = 0;
			*Data = 0;
		}
	}

	void Copy(const T* String)
	{
		Copy(String, (SizeType) StringUtility::Len(String));
	}

	void Copy(std::initializer_list<T> InitList)
	{
		Destruct();
		Allocate((SizeType) InitList.size());
		this->Size = (SizeType) InitList.size();

		SizeType i = 0;
		for (const T& Element : InitList)
		{
			Data[i++] = Element;
		}
	}

	void Copy(const TString& Other, bool bCopyAllocator = true)
	{
		Destruct();
		Copy(Other.Data, Other.Size);

		if (bCopyAllocator)
		{
			Allocator = Other.Allocator;
		}
	}

	void Move(TString&& Other, bool bMoveAllocator = true) noexcept
	{
		Destruct();

		Data = Other.Data;
		Size = Other.Size;
		Capacity = Other.Capacity;

		if (bMoveAllocator)
		{
			Allocator = MoveTemp(Other.Allocator);
		}

		Other.Data = nullptr;
		Other.Size = 0;
		Other.Capacity = 0;
		Other.Allocator = AllocatorType();
	}

	void Destruct()
	{
		if (Data)
		{
			Allocator.DeAllocate(Data, Capacity);
			Data = nullptr;
		}

		Size = 0;
		Capacity = 0;
	}

	void InsertNullTerminator()
	{
		Data[Size] = 0;
	}

	SizeType GetCapacityForSize(SizeType Size)
	{
		SizeType CalculatedCap = 0;

		while (CalculatedCap <= Size)
		{
			CalculatedCap += BlockSize;
		}

		return CalculatedCap + 1; // Make sure there is room for the null terminator.
	}
private:
	T* Data = nullptr;
	SizeType Size = 0, Capacity = 0;
	AllocatorType Allocator;
};

template <typename OStream, typename T>
inline OStream& operator<<(OStream& Stream, const TString<T>& String)
{
	Stream << *String;
	return Stream;
}

typedef TString<char> FString;

// UNSUPPORTED FOR NOW DUE TO wchar_t OVERLOAD OF TStringUtility NOT EXISTING!
typedef TString<wchar_t> FWideString;

// Hash specialization C strings
template <>
class THash<char*>
{
public:
	uint_t Hash(const char* Value) const
	{
		uint_t HashValue = 0x1505;

		char Char;
		while (Char = *Value++)
		{
			HashValue = ((HashValue << 5) + HashValue) + Char;
		}

		return HashValue;
	}
};

// Hash specialization for "TString"
template <typename T>
class THash<TString<T>>
{
public:
	uint_t Hash(const FString& Value) const
	{
		uint_t HashValue = 0x1505;

		const FString::CharType* String = Value.Get();
		FString::CharType Char;

		while (Char = *String++)
		{
			HashValue = ((HashValue << 5) + HashValue) + Char;
		}

		return HashValue;
	}
};

namespace std
{
	// STL-support for tString hashing. Internally uses THash to generate the hash.
	template <typename T>
	struct hash<TString<T>>
	{
		size_t operator()(const FString& Value) const
		{
			THash<TString<T>> Hasher;
			return Hasher.Hash(Value);
		}
	};
}
