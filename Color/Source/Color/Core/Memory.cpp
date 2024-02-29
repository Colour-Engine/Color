#include "ColorPCH.h"
#include "Memory.h"

void* FMemory::Copy(void* Dest, const void* Src, uint_t Count)
{
	int8* DestPtr = (int8*) Dest;
	const int8* SrcPtr = (const int8*) Src;

	while (Count--)
	{
		*DestPtr++ = *SrcPtr++;
	}

	return Dest;
}

void* FMemory::Move(void* Dest, const void* Src, uint_t Count)
{
	int8* DestPtr = (int8*) Dest;
	const int8* SrcPtr = (const int8*) Src;

	if (Dest <= Src)
	{
		while (Count--)
		{
			*DestPtr++ = *SrcPtr++;
		}
	}
	else
	{
		DestPtr += Count - 1;
		SrcPtr += Count - 1;

		while (Count--)
		{
			*DestPtr-- = *SrcPtr--;
		}
	}

	return Dest;
}
