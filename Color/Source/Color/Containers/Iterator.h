#pragma once

// TContainerType must have:
//   [ANY] RemoveAt(SizeType)
//   ElementType* Get()
//   bool IsValidIndex(SizeType) const
//   SizeType Num() const
//   ElementType& operator[](SizeType)
template <typename TContainerType, typename TElementType, typename TSizeType>
class TIndexedContainerIterator
{
public:
	using ContainerType = TContainerType;
	using ElementType = TElementType;
	using SizeType = TSizeType;
public:
	TIndexedContainerIterator(ContainerType& Container, SizeType StartPos = 0)
		: Container(Container), Index(StartPos)
	{
	}

	TIndexedContainerIterator& operator++()
	{
		Index++;
		return *this;
	}

	TIndexedContainerIterator& operator++(int)
	{
		TIndexedContainerIterator Temp = *this;
		Index++;
		return Temp;
	}

	TIndexedContainerIterator& operator--()
	{
		Index--;
		return *this;
	}

	TIndexedContainerIterator& operator--(int)
	{
		TIndexedContainerIterator Temp = *this;
		Index--;
		return Temp;
	}

	TIndexedContainerIterator& operator+=(SizeType Margin)
	{
		Index += Margin;
		return *this;
	}

	TIndexedContainerIterator& operator-=(SizeType Margin)
	{
		Index -= Margin;
		return *this;
	}

	TIndexedContainerIterator operator+(SizeType Margin) const
	{
		TIndexedContainerIterator Temp = *this;
		return Temp += Margin;
	}

	TIndexedContainerIterator operator-(SizeType Margin) const
	{
		TIndexedContainerIterator Temp = *this;
		return Temp -= Margin;
	}

	ElementType& operator*() const
	{
		return Container[Index];
	}

	ElementType& operator->() const
	{
		return Container.Get() + Index;
	}

	void RemoveCurrent()
	{
		Container.RemoveAt(Index);
		Index--;
	}

	void Reset()
	{
		Index = 0;
	}

	void SetToEnd()
	{
		Index = Container.Num();
	}

	SizeType GetIndex() const
	{
		return Index;
	}

	explicit operator bool() const
	{
		return Container.IsValidIndex(Index);
	}

	bool operator==(const TIndexedContainerIterator& Other) const
	{
		return &Container == &Other.Container && Index == Other.Index;
	}

	bool operator!=(const TIndexedContainerIterator& Other) const
	{
		return &Container != &Other.Container || Index != Other.Index;
	}
private:
	ContainerType& Container;
	SizeType Index = 0;
};
