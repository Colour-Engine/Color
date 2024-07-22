#pragma once

#include "Templates/Detail/RefControlBlock.h"
#include "Templates/Utility.h"

template <typename T>
class TWeakRef;

/**
 * Ref is a shared pointer tasked with the lifetime management of a dynamically-allocated object.
 * It will only free the object whenever all of the Ref instances owning the same object go out of scope.
 * It is pretty much just a shared pointer.
 */
template <typename T>
class TRef
{
public:
	using ElementType = T;
	using WeakType = TWeakRef<T>;
public:
	TRef() = default;

	TRef(TYPE_OF_NULLPTR)
		: TRef() { }

	template <typename U>
	explicit TRef(U* InPointer)
	{
		Copy(InPointer);
	}

	TRef(const TRef& Other)
	{
		Copy(Other);
	}

	template <typename U>
	TRef(const TRef<U>& Other)
	{
		Copy(Other);
	}
	
	TRef(TRef&& Other) noexcept
	{
		Move(MoveTemp(Other));
	}

	template <typename U>
	TRef(TRef<U>&& Other) noexcept
	{
		Move(MoveTemp(Other));
	}

	template <typename U>
	TRef(const TWeakRef<U>& WeakRef)
	{
		ControlBlock = WeakRef.GetControlBlock();
		ControlBlock->uRefs++;
	}

	~TRef()
	{
		DiscardCurrentControlBlock(true);
	}

	TRef& operator=(TYPE_OF_NULLPTR)
	{
		DiscardCurrentControlBlock(true);
		return *this;
	}

	TRef& operator=(const TRef& Other)
	{
		Copy(Other);
		return *this;
	}

	template <typename U>
	TRef& operator=(const TRef<U>& Other)
	{
		Copy(Other);
		return *this;
	}

	TRef& operator=(TRef&& Other) noexcept
	{
		Move(MoveTemp(Other));
		return *this;
	}

	template <typename U>
	TRef& operator=(TRef<U>&& Other) noexcept
	{
		Move(MoveTemp(Other));
		return *this;
	}

	void Reset()
	{
		DiscardCurrentControlBlock(true);
	}

	template <typename U>
	void Reset(U* InPointer)
	{
		DiscardAndCreateNewControlBlock((void*) InPointer, 1, 0);
	}

	void Swap(TRef& Other)
	{
		::Swap(ControlBlock, Other.ControlBlock);
	}

	T* Get() const { return (T*) ControlBlock->pObject; }
	T* operator->() const { return Get(); }
	T& operator*() const { return *Get(); }

	// Returns the number of shared TRef objects.
	uint32 GetRefCount() const { return ControlBlock->uRefs; }

	// Returns the amount of TWeakRefs holding references to the shared instance.
	uint32 GetWeakRefCount() const { return ControlBlock->uWeakRefs; }

	// Checks if this TRef instance is the only one sharing the managed object. (I.e. Refs == 1)
	bool IsUnique() const { return GetRefCount() == 1; }

	bool IsValid() const { return Get(); }
	operator bool() const { return IsValid(); }

	template <typename U>
	bool operator==(const TRef<U>& Other) const
	{
		return ControlBlock->pObject == Other.GetControlBlock()->pObject;
	}

	FRefControlBlock* GetControlBlock() const { return ControlBlock; }
	FRefControlBlock*& __internal_get_control_block_reference() { return ControlBlock; }
private:
	template <typename U>
	void Copy(U* InPointer)
	{
		DiscardAndCreateNewControlBlock((void*) InPointer, 1, 0);
	}

	template <typename U>
	void Copy(const TRef<U>& Other)
	{
		DiscardAndCreateNewControlBlock(Other.GetControlBlock()->pObject, ++Other.GetControlBlock()->uRefs, Other.GetControlBlock()->uWeakRefs);
	}

	template <typename U>
	void Move(TRef<U>&& Other) noexcept
	{
		DiscardCurrentControlBlock();

		ControlBlock = Other.GetControlBlock();
		Other.__internal_get_control_block_reference() = nullptr;
	}

	void CreateNewControlBlock(void* pObject = nullptr, uint32 uRefs = 0, uint32 uWeakRefs = 0)
	{
		ControlBlock = new FRefControlBlock { pObject, uRefs, uWeakRefs };
	}

	void DiscardCurrentControlBlock(bool bObjectFreeingRuleApplies = true)
	{
		if (ControlBlock)
		{
			if (--ControlBlock->uRefs == 0)
			{
				if (bObjectFreeingRuleApplies)
				{
					delete ControlBlock->pObject;
						   ControlBlock->pObject = nullptr;
				}

				if (ControlBlock->uWeakRefs == 0)
				{
					delete ControlBlock;
				}

				ControlBlock = nullptr;
			}
		}
	}

	void DiscardAndCreateNewControlBlock(void* pObject = nullptr, uint32 uRefs = 0, uint32 uWeakRefs = 0)
	{
		DiscardCurrentControlBlock();
		CreateNewControlBlock(pObject, uRefs, uWeakRefs);
	}
private:
	FRefControlBlock* ControlBlock = nullptr;
};

template <typename T, typename... Args>
constexpr TRef<T> MakeRef(Args&&... Arguments)
{
	return TRef<T>(new T(Forward<Args>(Arguments)...));
}

// Hash support for TRef.
template <typename T>
class THash<TRef<T>>
{
public:
	uint_t Hash(const TRef<T>& Value) const
	{
		return THash<typename TRef<T>::ElementType*>(Value.Get()).Hash();
	}
};
