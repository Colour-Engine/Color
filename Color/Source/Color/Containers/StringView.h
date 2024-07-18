#pragma once

#include "Core/Assert.h"
#include "Containers/String.h"
#include "Utils/StringUtility.h"

template <typename T, typename TSizeType = uint_t>
class TStringView
{
public:
	using CharType      = T;
	using SizeType      = T;
	using StringUtility = TStringUtility<T>;
	using Iterator      = TIndexedContainerIterator<TStringView, T, TSizeType>;
	using ConstIterator = TIndexedContainerIterator<const TStringView, const T, TSizeType>;
public:
	TStringView() = default;
	TStringView(TString<T>&& String) = delete;

	TStringView(const T* String, uint_t Size)
		: Data(String), Size(Size) { }

	TStringView(const T* String)
		: Data(String), Size((TSizeType) StringUtility::Len(String)) { }
	
	TStringView(const TString<T>& String)
		: Data(String.Get()), Size(String.Len()) { }

	int32 Compare(const T* String) const
	{
		return StringUtility::Cmp(Data, String);
	}

	int32 Compare(const TStringView& Other) const
	{
		return Compare(Other.Data);
	}

	int32 Compare(const TString<T>& Other) const
	{
		return Compare(Other.Get());
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
	const T* operator*() const { return Data; }

	// NOTE: Doesn't consider the Capacity of string, only calculates with the Size of it!
	SizeType GetAllocatedSize() const { return Size * sizeof(T); }
	SizeType GetMaxIndex() const { return Size > 0 ? Size - 1 : 0; }

	// For container support. Prefer using Len() when working with strings.
	SizeType Num() const { return Size; }
	SizeType Len() const { return Size; }
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
		verifyf(IsValidIndex(Index), "Index out of bounds!");
		return Data[Index];
	}

	const T& operator[](SizeType Index) const { return Data[Index]; }
private:
	const T* Data = nullptr;
	uint_t Size = 0;
};

// THash specialization for TStringView
template <typename T>
class THash<TStringView<T>>
{
public:
	uint_t Hash(const TStringView<T>& Value) const
	{
		uint_t HashValue = 0x1505;

		const T* String = Value.Get();
		T Char;

		while (Char = *String++)
		{
			HashValue = ((HashValue << 5) + HashValue) + Char;
		}

		return HashValue;
	}
};

typedef TStringView<char>    FStringView;
typedef TStringView<char>    FAnsiStringView;
typedef TStringView<wchar_t> FWideStringView;
