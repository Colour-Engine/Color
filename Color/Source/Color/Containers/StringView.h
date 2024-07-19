#pragma once

#include "Core/Assert.h"
#include "Containers/String.h"
#include "Utils/StringUtility.h"

template <typename T, typename TSizeType = uint_t>
class TStringView
{
public:
	using CharType      = T;
	using SizeType      = TSizeType;
	using StringUtility = TStringUtility<T>;
	using Iterator      = TIndexedContainerIterator<TStringView, T, TSizeType>;
	using ConstIterator = TIndexedContainerIterator<const TStringView, const T, TSizeType>;

	template <typename TAllocator>
	using TStringType   = TString<T, TSizeType, TAllocator>;

public:
	static constexpr SizeType Npos = TNumericLimits<SizeType>::Max();
	using AlgorithmLibType = StringAlgorithmLib<T, TSizeType, Npos>;
public:
	TStringView() = default;
	TStringView(const TStringView&) = default;
	TStringView& operator=(const TStringView&) = default;

	TStringView(TYPE_OF_NULLPTR) = delete;
	TStringView& operator=(TYPE_OF_NULLPTR) = delete;

	TStringView(const T* String, uint_t Size)
		: Data(String), Size(Size) { }

	TStringView(const T* String)
		: Data(String), Size((TSizeType) StringUtility::Len(String)) { }
	
	template <typename TAllocator>
	TStringView(const TStringType<TAllocator>& String)
		: Data(String.Get()), Size(String.Len()) { }

	template <typename TAllocator>
	TStringView& operator=(const TStringType<TAllocator>& String)
	{
		Data = String.Get();
		Size = String.Len();

		return *this;
	}

	void RemovePrefix(SizeType Count)
	{
		Data += Count;
		Size -= Count;
	}

	void RemoveSuffix(SizeType Count)
	{
		Size -= Count;
	}

	void Swap(TStringView& Other)
	{
		::Swap(Data, Other.Data);
		::Swap(Size, Other.Size);
	}

	TStringView Sub(SizeType Position = 0, SizeType Count = Npos) const
	{
		return TStringView { Data + Position, Count == Npos ? Size - Position : Count };
	}

	SizeType Find(T Char, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::FindFirstOccurenceOfChar(Data, Size, Char, Position, SearchCase);
	}

	SizeType Find(TStringView Substring, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::FindFirstOccurenceOfSubstring(Data, Size, Substring.Data, Substring.Size, Position, SearchCase);
	}

	SizeType Rfind(T Char, SizeType Position = Npos, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::RfindFirstOccurenceOfChar(Data, Size, Char, Position, SearchCase);
	}

	SizeType Rfind(TStringView Substring, SizeType Position = Npos, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::RfindFirstOccurenceOfSubstring(Data, Size, Substring.Data, Substring.Size, Position, SearchCase);
	}

	SizeType FindFirstOf(T Char, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return Find(Char, Position, SearchCase);
	}

	SizeType FindFirstOf(TStringView Charset, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::FindFirstOfCharOccurenceInCharset(Data, Size, Charset.Data, Charset.Size, Position, SearchCase);
	}

	SizeType FindFirstNotOf(T Char, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::FindFirstCharNotOfOccurence(Data, Size, Char, Position, SearchCase);
	}

	SizeType FindFirstNotOf(TStringView Charset, SizeType Position = 0, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::FindFirstCharNotOfOccurrenceInCharset(Data, Size, Charset.Data, Charset.Size, Position, SearchCase);
	}

	SizeType FindLastOf(T Char, SizeType Position = Npos) const
	{
		return Rfind(Char, Position);
	}

	SizeType FindLastOf(TStringView Charset, SizeType Position = Npos, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::FindLastOfCharOccurrenceInCharset(Data, Size, Charset.Data, Charset.Size, Position, SearchCase);
	}

	SizeType FindLastNotOf(T Char, SizeType Position = Npos, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::FindLastCharNotOfOccurrence(Data, Size, Char, Position, SearchCase);
	}

	SizeType FindLastNotOf(TStringView Charset, SizeType Position = Npos, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::FindLastNotOfCharOccurrenceInCharset(Data, Size, Charset.Data, Charset.Size, Position, SearchCase);
	}

	bool StartsWith(T Char, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::StartsWithChar(Data, Size, Char, SearchCase);
	}

	bool StartsWith(TStringView Substring, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::StartsWith(Data, Size, Substring.Data, Substring.Size, SearchCase);
	}

	bool EndsWith(T Char, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::EndsWithChar(Data, Size, Char, SearchCase);
	}
	
	bool EndsWith(TStringView Substring, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::EndsWith(Data, Size, Substring.Data, Substring.Size, SearchCase);
	}

	bool ContainsAnyOf(TStringView Charset, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::ContainsAnyCharInCharset(Data, Size, Charset.Data, Charset.Size, SearchCase);
	}

	bool Contains(T Char, ESearchCase SearchCase = ESearchCase::CaseSensitive)
	{
		return AlgorithmLibType::ContainsChar(Data, Size, Char, SearchCase);
	}

	bool Contains(TStringView Substring, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::ContainsSubstring(Data, Size, Substring.Data, Substring.Size, SearchCase);
	}

	int32 Compare(TStringView Other) const
	{
		return Compare(Other.Data);
	}

	bool Equals(TStringView Other, ESearchCase SearchCase = ESearchCase::CaseSensitive) const
	{
		return AlgorithmLibType::EqualityCompare(Data, Size, Other.Data, Other.Size, SearchCase);
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
	SizeType MaxSize() const { return TNumericLimits<SizeType>::Max() - 1; }

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

	bool operator==(TStringView Other) const { return Equals(Other); }
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
