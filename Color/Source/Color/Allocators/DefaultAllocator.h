#pragma once

#include "Misc/MiscDefines.h"
#include "Misc/IntTypes.h"

#include "Templates/Utility.h"

#include <new>

template <typename T>
struct TDefaultAllocator
{
	using ValueType = T;

	TDefaultAllocator() = default;
	TDefaultAllocator(const TDefaultAllocator&) = default;
	TDefaultAllocator& operator=(const TDefaultAllocator&) = default;

	template <typename U>
	TDefaultAllocator(const TDefaultAllocator<U>&) { }

	template <typename U>
	TDefaultAllocator& operator=(const TDefaultAllocator<U>&) { return *this; }

	~TDefaultAllocator() = default;

	CL_NODISCARD T* Allocate(uint_t Count)
	{
		return (T*)::operator new(Count * sizeof(T));
	}

	void DeAllocate(T* Pointer, uint_t Count)
	{
		::operator delete(Pointer, Count * sizeof(T));
	}

	template <typename... Args>
	void Construct(T* Pointer, Args&&... Arguments)
	{
		::new((void*) Pointer) T(Forward<Args>(Arguments)...);
	}

	void Destruct(T* Pointer)
	{
		Pointer->~T();
	}

	bool operator==(const TDefaultAllocator&) const { return true; }
	bool operator!=(const TDefaultAllocator&) const { return false; }

	template <typename U>
	bool operator==(const TDefaultAllocator<U>&) const { return true; }

	template <typename U>
	bool operator!=(const TDefaultAllocator<U>&) const { return false; }
};
