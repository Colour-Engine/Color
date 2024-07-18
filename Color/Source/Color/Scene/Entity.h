#pragma once

#include "Core/Base.h"
#include "Core/Assert.h"

#include "Scene/Component.h"
#include "Scene/ECSTypes.h"

#include "Components/TransformComponent.h"

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
		const char* IDName = nullptr;
		FComponent* ComponentObject = nullptr;
	};
	std::unordered_map<const char*, FComponentData> Components;

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
		const char* IDName = T::GetIDName();

		bool bReplacing = false;
		if (HasComponent<T>())
		{
			delete Components[IDName].ComponentObject;
			bReplacing = true;
		}

		T* Component = new T(Forward<Args>(Arguments)...);
		Components[IDName] =
		{
			IDName,
			Component
		};

		Component->__Internal_init(RefID, Scene, IDName);
		Component->OnAttach(bReplacing);

		return *Component;
	}

	void AddComponentViaPtr(const char* IDName, FComponent* Ptr)
	{
		if (Components.contains(IDName))
		{
			CL_CORE_ERROR("AddComponentViaPtr ERROR: Component with IDName '%s' already exists.", IDName);
			return;
		}

		Components[IDName].IDName = IDName;
		Components[IDName].ComponentObject = Ptr;
	}
	
	void RemoveComponentWithIDName(const char* IDName)
	{
		if (!Components.contains(IDName))
		{
			CL_CORE_ERROR("An attempt to remove component with IDName=%s on Entity with RefID=%d was made. Specified Component with that TypeID doesn't exist on the entity.", *IDName, RefID);
			return;
		}

		FComponentData& Data = Components[IDName];
		FComponent* ComponentPtr = (FComponent*) Data.ComponentObject; // cast to generic FComponent for proper call to destructor
		
		ComponentPtr->OnDetach();
		delete ComponentPtr;

		Components.erase(IDName);
	}

	template <typename T>
	void RemoveComponent()
	{
		const char* IDName = T::GetIDName();
		RemoveComponentWithIDName(IDName);
	}

	void RemoveAllComponents()
	{
		while (!Components.empty())
		{
			RemoveComponentWithIDName((*Components.begin()).first);
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
		const char* IDName = T::GetIDName();

		if (!HasComponent<T>())
		{
			CL_CORE_FATAL("Failed to get component with IDName=%s. Specified component doesn't exist.", *IDName);
		}

		return *(T*) Components[IDName].ComponentObject;
	}

	template <typename T>
	bool HasComponent()
	{
		const char* IDName = T::GetIDName();
		return Components.contains(IDName);
	}

	static FEntityData* RetrieveData(EntityRef RefID, FScene* InScene);
};

// A reference class for ease-of-use functionality. Makes operations to the entity it references on the Scene it belongs to.
class FEntity
{
public:
	FEntity() = default;
	FEntity(const FEntity&) = default;
	FEntity& operator=(const FEntity&) = default;
	FEntity(EntityRef RefID, FScene* Scene);

	template <typename T, typename... Args>
	T& AddComponent(Args&&... Arguments)
	{
		return GetData()->AddComponent<T>(Forward<Args>(Arguments)...);
	}

	template <typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... Arguments)
	{
		return GetData()->AddOrReplaceComponent<T>(Forward<Args>(Arguments)...);
	}

	template <typename T>
	void RemoveComponent()
	{
		GetData()->RemoveComponent<T>();
	}

	void RemoveAllComponents()
	{
		GetData()->RemoveAllComponents();
	}

	template <typename T, typename... Args>
	T& GetOrAddComponent(Args&&... Arguments)
	{
		return GetData()->GetOrAddComponent<T>(Forward<Args>(Arguments)...);
	}

	template <typename T>
	T& GetComponent()
	{
		return GetData()->GetComponent<T>();
	}

	template <typename T>
	bool HasComponent()
	{
		return GetData()->HasComponent<T>();
	}

	// Special case getter for the TransformComponent.
	// Only exists because TransformComponent is so common.
	FTransformComponent& GetTransform()
	{
		checkf(HasComponent<FTransformComponent>(), "Entity has no TransformComponent!");
		return GetComponent<FTransformComponent>();
	}

	void SetName(const FString& NewName);

	const FString& GetName() const { return GetData()->Name; }
	EntityRef GetRef() const { return RefID; }
	FScene* GetScene() const { return Scene; }

	FEntityData* GetData() const;
	operator EntityRef() const { return GetRef(); }

	// Checks if the entity has a valid entity data. Invalid entity handles returned by operations don't have one.
	// This check also works after the entity has been destroyed since it also checks if the Ref of the data exists on the scene it belongs to.
	// If the scene has been free'd from memory, the behavior is undefined.
	bool IsValid() const;
private:
	EntityRef RefID = 0;
	FScene* Scene = nullptr;
};
