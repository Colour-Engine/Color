#pragma once

#include "Templates/Utility.h"
#include "Misc/IntTypes.h"

// The default deleter used by TScope, this is the single object version.
template <typename T>
struct TDefaultDelete
{
	TDefaultDelete() = default;
	TDefaultDelete(const TDefaultDelete&) = default;

	template <typename U>
	TDefaultDelete(const TDefaultDelete<U>&) { }

	template <typename U>
	TDefaultDelete& operator=(const TDefaultDelete<U>&) { return *this; }

	TDefaultDelete& operator=(const TDefaultDelete&) = default;

	void operator()(T* Pointer) const
	{
		delete Pointer;
	}
};

// The default deleter used by TScope, this is the array version.
template <typename T>
struct TDefaultDelete<T[]>
{
	TDefaultDelete() = default;
	TDefaultDelete(const TDefaultDelete<T[]>&) = default;

	template <typename U>
	TDefaultDelete(const TDefaultDelete<U[]>&) { }

	template <typename U>
	TDefaultDelete& operator=(const TDefaultDelete<U[]>&) { return *this; }

	TDefaultDelete& operator=(const TDefaultDelete<T[]>&) = default;

	template <typename U>
	void operator()(U* Pointer) const
	{
		delete[] Pointer;
	}
};

/**
 * Scope is a smart pointer that manages the lifetime of, in this case, because this is the singular version, an object.
 * Scope will free the object it manages once it goes out of scope.
 * It is pretty much just a unique pointer.
 * 
 * @tparam T The type of object this Scope will manage.
 * @tparam TDeleter The deleter to use in deletion operations. It MUST be an object and must expose "[ANY] operator()(T*) const".
 */
template <typename T, typename TDeleter = TDefaultDelete<T>>
class TScope
{
public:
	using ElementType = T;
	using DeleterType = TDeleter;
public:
	TScope(const TScope&) = delete;
	TScope& operator=(const TScope&) = delete;

	TScope() = default;
	TScope(TYPE_OF_NULLPTR) { }
	
	explicit TScope(T* InPointer)
		: Pointer(InPointer)
	{
	}

	TScope(T* InPointer, const TDeleter& InDeleter)
		: Pointer(InPointer), Deleter(InDeleter)
	{
	}

	TScope(T* InPointer, TDeleter&& InDeleter)
		: Pointer(InPointer), Deleter(MoveTemp(InDeleter))
	{
	}

	template <typename U, typename Dx>
	TScope(TScope<U, Dx>&& Other) noexcept
		: Pointer(Other.Get()), Deleter(MoveTemp(Other.GetDeleter()))
	{
		Other.Internal__GetPtrRef() = nullptr;
		Other.GetDeleter() = TDeleter();
	}

	TScope(TScope&& Other) noexcept
		: TScope(Other)
	{
	}

	~TScope()
	{
		if (Pointer)
		{
			Deleter(Pointer);
		}
	}

	TScope& operator=(TYPE_OF_NULLPTR)
	{
		if (Pointer)
		{
			Deleter(Pointer);
			Pointer = nullptr;
		}

		return *this;
	}

	template <typename U, typename Dx>
	TScope& operator=(TScope<U, Dx>&& Other) noexcept
	{
		if (Pointer)
		{
			Deleter(Pointer);
		}

		Pointer = Other.Get();
		Deleter = MoveTemp(Other.GetDeleter());

		Other.Internal__GetPtrRef() = nullptr;
		Other.GetDeleter() = TDeleter();

		return *this;
	}

	TScope& operator=(TScope&& Other) noexcept
	{
		return *this = Other;
	}

	void Reset(T* InPointer = nullptr)
	{
		T* Temp = Pointer;
		Pointer = InPointer;

		if (Temp)
		{
			Deleter(Pointer);
		}
	}

	T* Release()
	{
		T* Temp = Pointer;
		Pointer = nullptr;
		return Temp;
	}

	void Swap(TScope& Other)
	{
		::Swap(Pointer, Other.Pointer);
		::Swap(Deleter, Other.Deleter);
	}

	T& operator*() const { return *Pointer; }
	T* operator->() const { return Pointer; }

	T* Get() const { return Pointer; }
	const TDeleter& GetDeleter() const { return Deleter; }
	TDeleter& GetDeleter() { return Deleter; }

	explicit operator bool() const { return Pointer; }
	bool IsValid() const { return Pointer; }

	bool operator==(const TScope& Other) const { return Pointer == Other.Pointer; }
	bool operator!=(const TScope& Other) const { return Pointer != Other.Pointer; }

	T*& Internal__GetPtrRef() { return Pointer; }
private:
	T* Pointer = nullptr;
	TDeleter Deleter;
};

/**
 * Scope is a smart pointer that manages the lifetime of, in this case, because this is the array version, an array of objects.
 * Scope will free the array it manages once it goes out of scope.
 * It is pretty much just a unique pointer.
 * 
 * @tparam T The type of the elements this Scope will manage.
 * @tparam TDeleter The deleter to use in deletion operations. It MUST be an object and must expose "[ANY] operator()(T*) const".
 */
template <typename T, typename TDeleter>
class TScope<T[], TDeleter>
{
public:
	using ElementType = T;
	using DeleterType = TDeleter;
public:
	TScope(const TScope&) = delete;
	TScope& operator=(const TScope&) = delete;

	TScope() = default;
	TScope(TYPE_OF_NULLPTR) { }
	
	template <typename U>
	explicit TScope(U* InPointer)
		: Pointer((T*) InPointer)
	{
	}

	template <typename U>
	TScope(U* InPointer, const TDeleter& InDeleter)
		: Pointer((T*) InPointer), Deleter(InDeleter)
	{
	}

	template <typename U>
	TScope(U* InPointer, TDeleter&& InDeleter)
		: Pointer((T*) InPointer), Deleter(MoveTemp(InDeleter))
	{
	}

	template <typename U, typename Dx>
	TScope(TScope<U[], Dx>&& Other) noexcept
		: Pointer(Other.Get()), Deleter(MoveTemp(Other.GetDeleter()))
	{
		Other.Internal__GetPtrRef() = nullptr;
		Other.GetDeleter() = TDeleter();
	}

	TScope(TScope&& Other) noexcept
		: TScope(Other)
	{
	}

	~TScope()
	{
		if (Pointer)
		{
			Deleter(Pointer);
		}
	}

	TScope& operator=(TYPE_OF_NULLPTR)
	{
		if (Pointer)
		{
			Deleter(Pointer);
			Pointer = nullptr;
		}

		return *this;
	}

	template <typename U, typename Dx>
	TScope& operator=(TScope<U, Dx>&& Other) noexcept
	{
		if (Pointer)
		{
			Deleter(Pointer);
		}

		Pointer = Other.Get();
		Deleter = MoveTemp(Other.GetDeleter());

		Other.Internal__GetPtrRef() = nullptr;
		Other.GetDeleter() = TDeleter();

		return *this;
	}

	TScope& operator=(TScope&& Other) noexcept
	{
		return *this = Other;
	}

	template <typename U>
	void Reset(U* InPointer = nullptr)
	{
		T* Temp = Pointer;
		Pointer = (T*) InPointer;

		if (Temp)
		{
			Deleter(Pointer);
		}
	}

	void Reset(TYPE_OF_NULLPTR = nullptr)
	{
		if (Pointer)
		{
			Deleter(Pointer);
			Pointer = nullptr;
		}
	}

	T* Release()
	{
		T* Temp = Pointer;
		Pointer = nullptr;
		return Temp;
	}

	void Swap(TScope& Other)
	{
		::Swap(Pointer, Other.Pointer);
		::Swap(Deleter, Other.Deleter);
	}

	T& operator[](uint_t Index) const { return *(Pointer + Index); }

	T* Get() const { return Pointer; }
	const TDeleter& GetDeleter() const { return Deleter; }
	TDeleter& GetDeleter() { return Deleter; }

	explicit operator bool() const { return Pointer; }
	bool IsValid() const { return Pointer; }

	bool operator==(const TScope& Other) const { return Pointer == Other.Pointer; }
	bool operator!=(const TScope& Other) const { return Pointer != Other.Pointer; }

	T*& Internal__GetPtrRef() { return Pointer; }
private:
	T* Pointer = nullptr;
	TDeleter Deleter;
};

template <typename T, typename... Args>
constexpr TScope<T> MakeScope(Args&&... Arguments)
{
	return TScope<T>(new T(Forward<Args>(Arguments)...));
}

template <typename T>
constexpr TScope<T[]> MakeScope(uint_t Size)
{
	return TScope<T[]>(new T[Size]);
}
