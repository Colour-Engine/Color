#pragma once

#include "Core/Base.h"
#include "Core/Assert.h"

#include "Scene/Component.h"
#include "Scene/ECSTypes.h"

#include <unordered_map>

class FScene;

struct FEntityData
{
	// All entities have a name that's not unique and an identifier that's unique to them. Names and IDs aren't components, they're an integral part of entities.
	// Only the identifier (also called Ref or RefID) of an entity is enough to identify that exact Entity.

	FString Name = "";
	EntityRef RefID = 0;
	FScene* Scene = nullptr;

	struct FComponentData
	{
		ComponentID TypeID = 0;
		FComponent* Data = nullptr;
	};
	std::unordered_map<ComponentID, FComponentData> Components;

	template <typename T, typename... Args>
	T& AddComponent(Args&&... Arguments)
	{
		if (HasComponent<T>())
		{
			CL_CORE_FATAL("Couldn't add component with TypeID=%d. Specified component already exists.", GetComponentTypeID<T>());
		}

		return AddOrReplaceComponent<T>(Forward<Args>(Arguments)...);
	}

	template <typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... Arguments)
	{
		ComponentID TypeID = GetComponentTypeID<T>();

		bool bReplacing = false;
		if (HasComponent<T>())
		{
			delete Components[TypeID].Data;
			bReplacing = true;
		}

		T* Component = new T(Forward<Args>(Arguments)...);
		Components[TypeID] =
		{
			TypeID,
			Component
		};

		Component->__Internal_init(RefID, Scene, TypeID);
		Component->OnAttach(bReplacing);

		return *Component;
	}
	
	void RemoveComponentWithTypeID(ComponentID TypeID)
	{
		if (!Components.contains(TypeID))
		{
			CL_CORE_ERROR("An attempt to remove component with TypeID=%d on Entity with RefID=%d was made. Specified Component with that TypeID doesn't exist on the entity.", TypeID, RefID);
			return;
		}

		FComponentData& Data = Components[TypeID];
		FComponent* ComponentPtr = (FComponent*) Data.Data; // cast to generic FComponent for proper call to destructor
		
		ComponentPtr->OnDetach();
		delete ComponentPtr;

		Components.erase(TypeID);
	}

	template <typename T>
	void RemoveComponent()
	{
		ComponentID TypeID = GetComponentTypeID<T>();
		RemoveComponentWithTypeID(TypeID);
	}

	void RemoveAllComponents()
	{
		while (!Components.empty())
		{
			RemoveComponentWithTypeID((*Components.begin()).second.TypeID);
		}
	}

	template <typename T, typename... Args>
	T& GetOrAddComponent(Args&&... Arguments)
	{
		if (HasComponent<T>())
		{
			return GetComponent<T>();
		}
		else
		{
			return AddComponent<T>(Forward<Args>(Arguments)...);
		}
	}

	template <typename T>
	T& GetComponent()
	{
		ComponentID TypeID = GetComponentTypeID<T>();

		if (!HasComponent<T>())
		{
			CL_CORE_FATAL("Failed to get component with TypeID=%d. Specified component doesn't exist.", TypeID);
		}

		return *(T*) Components[TypeID].Data;
	}

	template <typename T>
	bool HasComponent()
	{
		ComponentID TypeID = GetComponentTypeID<T>();
		return Components.contains(TypeID);
	}

	static FEntityData* RetrieveData(EntityRef RefID, FScene* InScene);
};

// A reference class for ease-of-use functionality. Makes operations to the entity it references on the Scene it belongs to.
class FEntity
{
public:
	FEntity() = delete;
	FEntity(const FEntity&) = default;
	FEntity& operator=(const FEntity&) = default;
	FEntity(FEntityData* InData);

	template <typename T, typename... Args>
	T& AddComponent(Args&&... Arguments)
	{
		return Data->AddComponent<T>(Forward<Args>(Arguments)...);
	}

	template <typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... Arguments)
	{
		return Data->AddOrReplaceComponent<T>(Forward<Args>(Arguments)...);
	}

	template <typename T>
	void RemoveComponent()
	{
		Data->RemoveComponent<T>();
	}

	void RemoveAllComponents()
	{
		Data->RemoveAllComponents();
	}

	template <typename T, typename... Args>
	T& GetOrAddComponent(Args&&... Arguments)
	{
		return Data->GetOrAddComponent<T>(Forward<Args>(Arguments)...);
	}

	template <typename T>
	T& GetComponent()
	{
		return Data->GetComponent<T>();
	}

	template <typename T>
	bool HasComponent()
	{
		return Data->HasComponent<T>();
	}

	const FString& GetName() const { return Data->Name; }
	EntityRef GetRef() const { return Data->RefID; }
	FScene* GetScene() const { return Data->Scene; }

	FEntityData* GetData() const { return Data; }
	operator EntityRef() const { return GetRef(); }
private:
	FEntityData* Data = nullptr;
};
