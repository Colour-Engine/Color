#pragma once

#include "Core/Base.h"
#include "Templates/Utility.h"

#include <functional>

template <typename T, typename... Params>
uint_t GetFunctionAddress(std::function<T(Params...)> Function)
{
	typedef T(TFuncType)(Params...);
	TFuncType** FuncPointer = Function.template target<TFuncType*>();
	return (uint_t) *FuncPointer;
}

#define DECLARE_EVENT(Name, ...) typedef ::FEvent<__VA_ARGS__> Name
#define CALL_ON_OBJ(Object, Function) [Object](auto&&... Args) -> decltype(auto) { return Object->Function(Forward<decltype(Args)>(Args)...); }

template <typename... Params>
class FEvent
{
public:
	typedef std::function<void(Params...)> BindingType;
public:
	FEvent() = default;
	FEvent(const FEvent&) = default;
	FEvent& operator=(const FEvent&) = default;

	FEvent(std::initializer_list<BindingType> InBindings)
		: Bindings(InBindings)
	{
	}

	bool Bind(const BindingType& Binding)
	{
		if (IsBound(Binding))
		{
			return false;
		}

		Bindings.Push(Binding);
		return true;
	}

	bool Unbind(const BindingType& Binding)
	{
		uint32 Index = Find(Binding);

		if (Index == Bindings.NPos)
		{
			return false;
		}

		Bindings.RemoveAt(Index);
		return true;
	}

	void Broadcast(Params... Parameters) const
	{
		for (const BindingType& Binding : Bindings)
		{
			Binding(Parameters...);
		}
	}

	// Returns NPos if the item wasn't found.
	uint32 Find(const BindingType& Binding) const
	{
		for (uint32 i = 0; i < Bindings.Num(); i++)
		{
			const BindingType& Element = Bindings[i];

			if (GetFunctionAddress(Element) == GetFunctionAddress(Binding))
			{
				return i;
			}
		}

		return Bindings.NPos;
	}

	bool IsBound(const BindingType& Binding) const
	{
		for (const BindingType& Element : Bindings)
		{
			if (GetFunctionAddress(Element) == GetFunctionAddress(Binding))
			{
				return true;
			}
		}

		return false;
	}

	const TArray<BindingType>& GetBindings() const { return Bindings; }
private:
	TArray<BindingType> Bindings;
};
