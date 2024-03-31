#pragma once

#include "Allocators/DefaultAllocator.h"
#include "Templates/NumericLimits.h"

#include "Containers/Iterator.h"
#include "Core/Assert.h"
#include "Core/Memory.h"

#include <initializer_list>

template <typename T, typename TSizeType = uint32, typename TAllocatorType = TDefaultAllocator<T>>
class TArray
{
public:
	using ValueType     = T;
	using SizeType      = TSizeType;
	using AllocatorType = TAllocatorType;
	using Iterator      = TIndexedContainerIterator<TArray, T, TSizeType>;
	using ConstIterator = TIndexedContainerIterator<const TArray, const T, TSizeType>;
public:
	static constexpr SizeType NPos = TNumericLimits<SizeType>::Max();
	static constexpr SizeType BlockSize = 15;
public:
	TArray(const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Allocate(BlockSize, true);
	}

	TArray(const TArray& Other)
	{
		Copy(Other);
	}

	TArray(const TArray& Other, const AllocatorType& Allocator)
		: Allocator(Allocator)
	{
		Copy(Other, false);
	}

	TArray(TArray&& Other) noexcept
	{
		Move(MoveTemp(Other));
	}

	TArray(TArray&& Other, const AllocatorType& Allocator)
		: Allocator(Allocator)
	{
		Move(MoveTemp(Other), false);
	}

	TArray(const T* Pointer, SizeType Size, const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Copy(Pointer, Size);
	}

	TArray(std::initializer_list<T> InitList, const AllocatorType& Allocator = AllocatorType())
		: Allocator(Allocator)
	{
		Copy(InitList);
	}

	~TArray()
	{
		Destruct();
	}

	TArray& operator=(const TArray& Other)
	{
		Copy(Other);
		return *this;
	}

	TArray& operator=(TArray&& Other) noexcept
	{
		Move(MoveTemp(Other));
		return *this;
	}

	TArray& operator=(std::initializer_list<T> InitList)
	{
		Copy(InitList);
		return *this;
	}
	
	uint32 Add(const T& Item)
	{
		SizeType Index = Size++;

		GrowIfNecessary();
		Allocator.Construct(Data+(Index), Item);

		return Index;
	}

	uint32 Add(T&& Item)
	{
		SizeType Index = Size++;

		GrowIfNecessary();
		Allocator.Construct(Data+(Index), MoveTemp(Item));

		return Index;
	}

	void Push(const T& Item)
	{
		Add(Item);
	}

	void Push(T&& Item)
	{
		Add(MoveTemp(Item));
	}

	template <typename... Args>
	T& Emplace(Args&&... Arguments)
	{
		GrowIfNecessary();
		Allocator.Construct(Data+(Size++), Forward<Args>(Arguments)...);
		return Back();
	}

	SizeType RemoveAll(const T& Item)
	{
		SizeType NumRemoved = 0;
		SizeType Idx;

		while ((Idx = Find(Item)) != NPos)
		{
			RemoveAt(Idx);
			NumRemoved++;
		}
		
		return NumRemoved;
	}

	bool RemoveAt(SizeType Index)
	{
		if (!IsValidIndex(Index))
		{
			return false;
		}

		Allocator.Destruct(Data + Index);
		for (SizeType i = Index; i < Size - 1; i++)
		{
			Data[i] = MoveTemp(Data[i+1]);
		}
		Size--;

		return true;
	}

	bool Remove(const T& Item)
	{
		return RemoveAt(Find(Item));
	}

	void Pop()
	{
		if (Size > 0)
		{
			Allocator.Destruct(Data+((Size--)-1));
		}
	}

	void Resize(SizeType NewSize)
	{
		if (Size > NewSize)
		{
			SizeType PrevSize = Size;
			Size = NewSize;

			for (SizeType i = Size - 1; i < PrevSize; i++)
			{
				Allocator.Destruct(Data + i);
			}
		}
		else if (Size < NewSize)
		{
			SizeType Diff = NewSize - Size;

			if (NewSize > Capacity)
			{
				ReAllocate(NewSize);
			}

			while (Diff--)
			{
				Allocator.Construct(Data+(Size++), T());
			}
		}
	}

	void Reserve(SizeType EnsuredCapacity)
	{
		if (EnsuredCapacity > Capacity)
		{
			ReAllocate(EnsuredCapacity);
		}
	}

	// Makes the array shrink to the lowest allocation size it can while not losing any data.
	// Basically, it sets the capacity of the array to the size of it.
	void Shrink()
	{
		if (Capacity > Size)
		{
			ReAllocate(Size, true);
		}
	}

	void Clear()
	{
		for (SizeType i = 0; i < Size; i++)
		{
			Allocator.Destruct(Data+i);
		}

		Size = 0;
	}

	/**
	 * Sets a certain amount of elements, starting at the given position index to the given value.
	 * 
	 * @param Value The value to set the elements within the range to.
	 * @param Position The index at which the changes will start happening.
	 * @param Count Amount of elements to change. Leave as NPos if every elements in the array from the starting index is going to be changed.
	 */
	void SetRange(const T& Value, SizeType Position = 0, SizeType Count = NPos)
	{
		SizeType EndIndex = (Count != NPos ? Count : Size - Position) + Position;
		checkf(IsValidIndex(EndIndex), "TArray::SetRange was supplied with such parameters that'd result with the end of the range being out of bounds!")

		for (Position; Position < EndIndex; Position++)
		{
			Data[Position] = Value;
		}
	}

	// Sets every element within the array to the given value.
	// Basically, it just calls SetRange(Value, 0, NPos).
	void Fill(const T& Value)
	{
		SetRange(Value);
	}

	void Swap(TArray& Other)
	{
		::Swap(Data, Other.Data);
		::Swap(Size, Other.Size);
		::Swap(Capacity, Other.Capacity);
		::Swap(Allocator, Other.Allocator);
	}

	bool Compare(const TArray& Other) const
	{
		for (SizeType i = 0; i < Size; i++)
		{
			if (Data[i] != Other.Data[i])
			{
				return false;
			}
		}

		return true;
	}

	// Returns NPos if the item wasn't found.
	SizeType Find(const T& Item) const
	{
		for (SizeType i = 0; i < Size; i++)
		{
			if (Data[i] == Item)
			{
				return i;
			}
		}

		return NPos;
	}

	bool Contains(const T& Item) const
	{
		return Find(Item) != NPos;
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
	T* Get() { return Data; }

	const AllocatorType& GetAllocator() const { return Allocator; }
	AllocatorType& GetAllocator() { return Allocator; }

	SizeType GetAllocatedSize() const { return Capacity * sizeof(T); }
	SizeType MaxSize() const { return TNumericLimits<SizeType>::Max(); }
	SizeType GetMaxIndex() const { return Size > 0 ? Size - 1 : 0; }
	SizeType GetNPos() const { return NPos; }
	SizeType Num() const { return Size; }
	SizeType Max() const { return Capacity; }
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

	T& At(SizeType Index)
	{
		verifyf(IsValidIndex(Index), "Index out of bounds!");
		return Data[Index];
	}

	const T& operator[](SizeType Index) const
	{
		checkf(IsValidIndex(Index), "Index out of bounds!");
		return Data[Index];
	}

	T& operator[](SizeType Index)
	{
		checkf(IsValidIndex(Index), "Index out of bounds!");
		return Data[Index];
	}

	bool operator==(const TArray& Other) const
	{
		return Compare(Other);
	}
private:
	void Allocate(SizeType Cap, bool bStrictlyUseGivenSize = false)
	{
		Size = 0;
		Capacity = bStrictlyUseGivenSize ? Cap : GetCapacityForSize(Cap);
		Data = Allocator.Allocate(Capacity);
	}

	void ReAllocate(SizeType Cap, bool bStrictlyUseGivenSize = false)
	{
		SizeType PrevSize = Size;

		if (Size > Cap)
		{
			Size = Cap;
		}

		SizeType NewCapacity = bStrictlyUseGivenSize ? Cap : GetCapacityForSize(Cap);
		T* NewData = Allocator.Allocate(NewCapacity);

		for (SizeType i = 0; i < Size; i++)
		{
			Allocator.Construct(NewData + i, MoveTemp(Data[i]));
		}

		for (SizeType i = 0; i < PrevSize; i++)
		{
			Allocator.Destruct(Data + i);
		}

		Allocator.DeAllocate(Data, Capacity);
		Data = NewData;
		Capacity = NewCapacity;
	}

	void Grow()
	{
		// +1 triggers new block step.
		ReAllocate(Capacity + 1);
	}

	void GrowIfNecessary()
	{
		if (Size == Capacity)
		{
			Grow();
		}
	}

	void Copy(const T* Pointer, SizeType Size)
	{
		Destruct();
		Allocate(Size);
		this->Size = Size;

		for (SizeType i = 0; i < Size; i++)
		{
			Allocator.Construct(Data+i, Pointer[i]);
		}
	}

	void Copy(std::initializer_list<T> InitList)
	{
		Destruct();
		Allocate((SizeType) InitList.size());
		this->Size = (SizeType) InitList.size();

		SizeType i = 0;
		for (const T& Element : InitList)
		{
			Allocator.Construct(Data+i, Element);
			i++;
		}
	}

	void Copy(const TArray& Other, bool bCopyAllocator = true)
	{
		Destruct();
		Copy(Other.Data, Other.Size);

		if (bCopyAllocator)
		{
			Allocator = Other.Allocator;
		}
	}

	void Move(TArray&& Other, bool bMoveAllocator = true) noexcept
	{
		Destruct();

		Data = Other.Data;
		Size = Other.Size;
		Capacity = Other.Capacity;

		if (bMoveAllocator)
		{
			Allocator = MoveTemp(Other.Allocator);
		}
		
		Other.Data = nullptr;
		Other.Size = 0;
		Other.Capacity = 0;
		Other.Allocator = AllocatorType();
	}

	void Destruct()
	{
		if (Data)
		{
			for (SizeType i = 0; i < Size; i++)
			{
				Allocator.Destruct(Data + i);
			}

			Allocator.DeAllocate(Data, Capacity);
			Data = nullptr;
		}

		Size = 0;
		Capacity = 0;
	}

	SizeType GetCapacityForSize(SizeType Size)
	{
		SizeType CalculatedCap = 0;

		while (CalculatedCap <= Size)
		{
			CalculatedCap += BlockSize;
		}

		return CalculatedCap;
	}
private:
	T* Data = nullptr;
	SizeType Size = 0, Capacity = 0;
	AllocatorType Allocator;
};
