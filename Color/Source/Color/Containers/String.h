#pragma once

#include "Allocators/DefaultAllocator.h"
#include "Templates/NumericLimits.h"

#include "Core/Memory.h"
#include "Misc/NoInit.h"
#include "Utils/StringUtility.h"

#include <initializer_list>

template <typename T, typename TSizeType = uint32, typename TAllocatorType = TDefaultAllocator<T>>
class TString
{
public:
	using CharType = T;
	using SizeType = TSizeType;
	using AllocatorType = TAllocatorType;
	using StringUtility = TStringUtility<T>;
public:
	static constexpr SizeType NPos = TNumericLimits<SizeType>::Max();
	static constexpr SizeType BlockSize = 30;
public:
	TString(TYPE_OF_NULLPTR) = delete;
	TString& operator=(TYPE_OF_NULLPTR) = delete;
	TString(ENoInit) { }

	TString(const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Allocate(BlockSize, true);
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

	TString(T Char, uint32 Count, const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Allocate(Count);
		SetRange(Char, 0, Count);
	}

	TString(T Char, const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Allocate(1);
		*Data = Char;
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
		Allocate(1);
		*Data = Char;

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
		return Appended(InitList.begin(), (SizeType)InitList.size());
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
		if (Capacity < Cap)
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
	 * @param Count Amount of characters to change. Leave as NPos if every character in the array from the starting index is going to be changed.
	 */
	void SetRange(T Char, uint32 Position = 0, uint32 Count = NPos)
	{
		uint32 EndIndex = (Count != NPos ? Count : Size - Position) + Position;
		// TODO: Make sure IsValidIndex(EndIndex)

		for (Position; Position < EndIndex; Position++)
		{
			Data[Position] = Char;
		}
	}

	// Sets every character within the string to the given value.
	// Basically, it just calls SetRange(Value, 0, NPos).
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

	TString Sub(SizeType Position = 0, SizeType Count = NPos)
	{
		uint32 EndIndex = (Count != NPos ? Count : Size - Position) + Position;
		// TODO: Make sure IsValidIndex(EndIndex)

		return TString(Data + Position, Count == NPos ? Size - Position : Count);
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
		// TODO: Bounds checking
		return Data[Index];
	}

	T& At(SizeType Index)
	{
		// TODO: Bounds checking
		return Data[Index];
	}

	const T& operator[](SizeType Index) const { return Data[Index]; }
	T& operator[](SizeType Index) { return Data[Index]; }

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

		for (SizeType i = 0; i < Size; i++)
		{
			NewData[i] = Data[i];
		}

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
		if (Size == Capacity)
		{
			Grow();
		}
	}

	void Copy(const T* Pointer, SizeType Size)
	{
		Destruct();
		Allocate(Size);
		this->Size = Size;

		for (SizeType i = 0; i < Size; i++)
		{
			Data[i] = Pointer[i];
		}

		InsertNullTerminator();
	}

	void Copy(const T* String)
	{
		Copy(String, (SizeType) StringUtility::Len(String));
		InsertNullTerminator();
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
