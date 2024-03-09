#include "ColorPCH.h"
#include "StringUtility.h"

uint_t TStringUtility<char>::Len(const char* String)
{
	if (*String == 0)
	{
		return 0;
	}

	const char* Start = String;
	while (*++String);
	return (uint_t)(String - Start);
}

int32 TStringUtility<char>::Cmp(const char* LHS, const char* RHS)
{
	while (*LHS && *LHS == *RHS)
	{
		LHS++;
		RHS++;
	}

	return (int32)(ubyte)(*LHS) - (int32)(ubyte)(*RHS);
}
