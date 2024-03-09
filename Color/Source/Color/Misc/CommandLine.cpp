#include "ColorPCH.h"
#include "CommandLine.h"

#include "Utils/StringUtility.h"

FCommandLine::FCommandLine(char** InList, uint32 InSize)
	: List(InList), Size(InSize)
{
}

FCommandLine::~FCommandLine()
{
}

bool FCommandLine::Contains(const char* Argument) const
{
	for (uint32 i = 0; i < Size; i++)
	{
		if (FStringUtility::Cmp(List[i], Argument) == 0)
		{
			return true;
		}
	}

	return false;
}

uint32 FCommandLine::Find(const char* Argument) const
{
	for (uint32 i = 0; i < Size; i++)
	{
		if (FStringUtility::Cmp(List[i], Argument) == 0)
		{
			return i;
		}
	}

	return InvalidIndex;
}

uint32 FCommandLine::Count(const char* Argument) const
{
	uint32 Count{};

	for (uint32 i = 0; i < Size; i++)
	{
		if (FStringUtility::Cmp(List[i], Argument) == 0)
		{
			Count++;
		}
	}

	return Count;
}

char* FCommandLine::At(uint32 Index) const
{
	verifyf(IsValidIndex(Index), "Index out of bounds");
	return List[Index];
}
