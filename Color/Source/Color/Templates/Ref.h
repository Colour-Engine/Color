#pragma once

#include "Misc/IntTypes.h"
#include "Templates/Utility.h"

/**
 * Ref is a shared pointer tasked with the lifetime management of a dynamically-allocated object.
 * It will only free the object whenever all of the Ref instances owning the same object go out of scope.
 * It is pretty much just a shared pointer.
 */
template <typename T>
class TRef
{
public:
	typedef T ElementType;
public:
	TRef()
	{
		AllocNewRefs(0);
	}

	TRef(TYPE_OF_NULLPTR)
		: TRef()
	{
	}

	template <typename U>
	explicit TRef(U* InPointer)
	{
		Pointer = InPointer;
		AllocNewRefs(1);
	}

	TRef(const TRef& Other)
	{
		Copy(Other);
	}

	TRef(TRef&& Other) noexcept
	{
		Move(MoveTemp(Other));
	}

	template <typename U>
	TRef(const TRef<U>& Other)
	{
		Copy(Other);
	}

	template <typename U>
	TRef(TRef<U>&& Other) noexcept
	{
		Move(MoveTemp(Other));
	}

	~TRef()
	{
		Destruct();
	}

	TRef& operator=(const TRef& Other)
	{
		Copy(Other);
		return *this;
	}

	TRef& operator=(TRef&& Other) noexcept
	{
		MoveIn(MoveTemp(Other));
		return *this;
	}

	template <typename U>
	TRef& operator=(const TRef<U>& Other)
	{
		Copy(Other);
		return *this;
	}

	template <typename U>
	TRef& operator=(TRef<U>&& Other) noexcept
	{
		MoveIn(MoveTemp(Other));
		return *this;
	}

	void Reset()
	{
		Pointer = nullptr;
		AllocNewRefs(0);
	}

	template <typename U>
	void Reset(U* InPointer)
	{
		Pointer = InPointer;
		AllocNewRefs(1);
	}

	void Swap(TRef& Other)
	{
		::Swap(Pointer, Other.Pointer);
		::Swap(Refs, Other.Refs);
	}

	T& operator*()  const { return *Pointer; }
	T* operator->() const { return  Pointer; }

	T* Get() const { return Pointer; }
	uint32 GetRefCount() const { return *Refs; }
	bool IsUnique() const { return *Refs == 0; }

	template <typename U>
	bool operator==(const TRef<U>& Other) const
	{
		return Pointer == Other.Pointer;
	}

	explicit operator bool() const { return Pointer; }

	T*& __Internal_GetPtr() { return Pointer; }
	uint32*& __Internal_GetControlBlock() { return Refs; }
private:
	template <typename U>
	void Copy(const TRef<U>& Other)
	{
		Destruct();

		Pointer = (T*) Other.Get();
		Refs = Other.__Internal_GetControlBlock();

		++*Refs;
	}

	template <typename U>
	void Move(TRef<U>&& Other) noexcept
	{
		Destruct();

		Pointer = (T*) Other.Get();
		Refs = Other.__Internal_GetControlBlock();

		Other.__Internal_GetPtr() = nullptr;
		Other.__Internal_GetControlBlock() = nullptr;
	}

	void Destruct()
	{
		if (Refs)
		{
			if (Pointer)
			{
				if ((Refs == 0 || (--*Refs == 0)))
				{
					delete Pointer;
					delete Refs;

					Pointer = nullptr;
					Refs = nullptr;
				}
			}
			else
			{
				delete Refs;
				Refs = nullptr;
			}
		}
	}

	void AllocNewRefs(uint32 NewRefCount)
	{
		if (Refs)
		{
			if (Refs == 0)
			{
				*Refs = NewRefCount;
			}
			else
			{
				if (--*Refs == 0)
				{
					delete Pointer;
				}

				Refs = new uint32(NewRefCount);
			}
		}
		else
		{
			Refs = new uint32(NewRefCount);
		}
	}
private:
	T* Pointer = nullptr;
	uint32* Refs = nullptr;
};

template <typename T, typename... Args>
constexpr TRef<T> MakeRef(Args&&... Arguments)
{
	return TRef<T>(new T(Forward<Args>(Arguments)...));
}
