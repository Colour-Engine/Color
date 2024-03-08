#pragma once

#include "Containers/Iterator.h"
#include "Misc/IntTypes.h"

struct FCommandLine
{
	static constexpr uint32 InvalidIndex = 0xffffffff;

	using Iterator = TIndexedContainerIterator<FCommandLine, char**, uint32>;
	using ConstIterator = TIndexedContainerIterator<const FCommandLine, const char**, uint32>;

	char** List = nullptr;
	uint32 Size = 0;

	FCommandLine() = default;
	FCommandLine(const FCommandLine&) = default;
	FCommandLine& operator=(const FCommandLine&) = default;

	FCommandLine(char** InList, uint32 InSize);
	~FCommandLine();

	bool Contains(const char* Argument) const;
	uint32 Find(const char* Argument) const;
	uint32 Count(const char* Argument) const;

	// For iterator support:
	void RemoveAt(uint32 Index) { }
	char** Get() const { return List; }
	bool IsValidIndex(uint32 Index) const { return Index > -1 && Index < Size; }
	uint32 Num() const { return Size; }

	Iterator CreateIterator()
	{
		Iterator It(*this);
		return It;
	}

	ConstIterator CreateConstIterator() const
	{
		ConstIterator It(*this);
		return It;
	}

	Iterator begin() { return CreateIterator(); }
	Iterator end() { Iterator It = CreateIterator(); It.SetToEnd(); return It; }

	ConstIterator begin() const { return CreateConstIterator(); }
	ConstIterator end() const { ConstIterator It = CreateConstIterator(); It.SetToEnd(); return It; }

	ConstIterator cbegin() const { return begin(); }
	ConstIterator cend() const { return end(); }

	char* At(uint32 Index) const;
	char* operator[](uint32 Index) const { return List[Index]; }
};
