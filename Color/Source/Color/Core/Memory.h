#pragma once

#include "Misc/IntTypes.h"

struct FMemory
{
	static void* Copy(void* Dest, const void* Src, uint_t Count);
	static void* Move(void* Dest, const void* Src, uint_t Count);
};

// Helper type to manipulate a block of memory.
// FBuffer is not a smart pointer, it just holds a pointer to a memory block and stores the size of it.
// Useful for manipulating the block of memory.
struct FBuffer
{
	uint8* Data = nullptr;
	uint_t Size = 0;

	FBuffer() = default;
	FBuffer(const FBuffer&) = default;
	FBuffer& operator=(const FBuffer&) = default;

	// Allocates memory of InSize and sets the data pointer to that.
	FBuffer(uint_t InSize)
	{
		Allocate(InSize);
	}

	FBuffer(void* InData, uint_t InSize)
		: Data((uint8*) InData), Size(InSize)
	{
	}

	static FBuffer Copy(FBuffer Buffer)
	{
		FBuffer Result(Buffer.Size);
		FMemory::Copy(Result.Data, Buffer.Data, Buffer.Size);
		return Result;
	}

	void Allocate(uint_t Size)
	{
		this->Size = Size;
		Data = new uint8[Size];
	}

	// Stops pointing to the memory block. The buffer will be invalidated. Returns the Data pointer before the invalidation.
	uint8* Release()
	{
		uint8* Temp = Data;

		Data = nullptr;
		Size = 0;

		return Temp;
	}

	// Frees (deletes) the memory block and invalidates the buffer.
	void FreeAndRelease()
	{
		delete Data;
		Data = nullptr;
		Size = 0;
	}

	template <typename T>
	const T* As() const
	{
		return static_cast<const T*>(Data);
	}

	template <typename T>
	T* As()
	{
		return static_cast<T*>(Data);
	}

	bool IsValid() const
	{
		return Data != nullptr;
	}
};
