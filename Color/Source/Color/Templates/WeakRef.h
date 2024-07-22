#pragma once

#include "Templates/Ref.h"

template <typename T>
class TWeakRef
{
public:
	using ElementType = T;
public:
	TWeakRef() = default;

	TWeakRef(const TWeakRef& Other)
	{
		Copy(Other);
	}

	template <typename U>
	TWeakRef(const TWeakRef<U>& Other)
	{
		Copy(Other);
	}

	TWeakRef(TWeakRef&& Other) noexcept
	{
		Move(MoveTemp(Other));
	}
	
	template <typename U>
	TWeakRef(TWeakRef<U>&& Other) noexcept
	{
		Move(MoveTemp(Other));
	}

	template <typename U>
	TWeakRef(const TRef<U>& Ref)
	{
		Copy(Ref);
	}

	~TWeakRef()
	{
		LeaveControlBlock();
	}

	TWeakRef& operator=(const TWeakRef& Other)
	{
		Copy(Other);
		return *this;
	}

	template <typename U>
	TWeakRef& operator=(const TWeakRef<U>& Other)
	{
		Copy(Other);
		return *this;
	}

	TWeakRef& operator=(TWeakRef&& Other) noexcept
	{
		Move(MoveTemp(Other));
		return *this;
	}

	template <typename U>
	TWeakRef& operator=(TWeakRef<U>&& Other) noexcept
	{
		Move(MoveTemp(Other));
		return *this;
	}

	template <typename U>
	TWeakRef& operator=(const TRef<U>& Ref)
	{
		Copy(Ref);
		return *this;
	}

	void Reset()
	{
		LeaveControlBlock();
	}

	void Swap(TWeakRef& Other)
	{
		::Swap(ControlBlock, Other.ControlBlock);
	}

	/**
	 * The method to use to access the object, creates a TRef for safe access.
	 * If the object has expired, an empty TRef is returned.
	 * If not, a TRef with the object being equal to the desired object pointer is returned.
	 */
	TRef<T> Access() const
	{
		return HasExpired() ? TRef<T>() : TRef<T>(*this);
	}

	// Returns the number of shared TRef objects.
	uint32 GetRefCount() const { return ControlBlock->uRefs; }

	// Returns the amount of TWeakRefs holding references to the shared instance.
	uint32 GetWeakRefCount() const { return ControlBlock->uWeakRefs; }

	// Checks whether or not the object pointed to by the weak ref has been free'd (i.e. all TRef shareholders have gone out of scope)
	bool HasExpired() const { return GetRefCount() == 0; }

	FRefControlBlock* GetControlBlock() const { return ControlBlock; }
	FRefControlBlock*& __internal_get_control_block_reference() { return ControlBlock; }
public:
	template <typename U>
	void Copy(const TRef<U>& Ref)
	{
		LeaveAndJoinControlBlock(Ref.GetControlBlock());
	}

	template <typename U>
	void Copy(const TWeakRef<U>& Other)
	{
		LeaveAndJoinControlBlock(Other->GetControlBlock());
	}

	template <typename U>
	void Move(TWeakRef<U>&& Other) noexcept
	{
		LeaveControlBlock();

		ControlBlock = Other.GetControlBlock();
		Other.__internal_get_control_block_reference() = nullptr;
	}

	void JoinControlBlock(FRefControlBlock* ControlBlock)
	{
		this->ControlBlock = ControlBlock;
		this->ControlBlock->uWeakRefs++;
	}

	void LeaveControlBlock()
	{
		if (ControlBlock)
		{
			if (--ControlBlock->uWeakRefs == 0 && ControlBlock->uRefs == 0)
			{
				delete ControlBlock;
			}

			ControlBlock = nullptr;
		}
	}

	void LeaveAndJoinControlBlock(FRefControlBlock* ControlBlock)
	{
		LeaveControlBlock();
		JoinControlBlock(ControlBlock);
	}
private:
	FRefControlBlock* ControlBlock = nullptr;
};
