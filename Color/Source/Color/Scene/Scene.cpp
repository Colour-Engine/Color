#include "ColorPCH.h"
#include "Scene.h"

#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

FScene::FScene()
{
}

FScene::~FScene()
{
	for (auto&& [RefID, Entity] : Entities)
	{
		Entity.RemoveAllComponents();
	}

	Entities.clear();
}

void FScene::TickScene(float DeltaTime)
{
	for (auto&& [Ref, Entity] : Entities)
	{
		for (auto&& [TypeID, Component] : Entity.Components)
		{
			if (Component.Data->IsTickEnabled())
			{
				Component.Data->OnTick(DeltaTime);
			}
		}
	}
}

FEntity FScene::CreateEntityWithRefID(EntityRef RefID, const FString& Name)
{
	if (Entities.contains(RefID))
	{
		CL_CORE_ERROR("Failed to create entity '%s' with the RefID: %d, another entity with that ID already exists.", *Name, RefID);
		return { nullptr };
	}

	Entities[RefID] =
	{
		Name,
		RefID,
		this,
		{
		}
	};
	Entities[RefID].AddComponent<FTransformComponent>();

	return { &Entities[RefID] };
}

FEntity FScene::CreateEntity(const FString& Name)
{
	return CreateEntityWithRefID(NextEntityRef(), Name);
}

FEntity FScene::DuplicateEntity(FEntity Entity)
{
	if (!Internal_ValidateEntityScene(Entity))
	{
		CL_CORE_ERROR("Tried to duplicate an entity that didn't belong to that scene.", Entity.GetRef());
		return { nullptr };
	}

	return DuplicateEntity(Entity.GetRef());
}

FEntity FScene::DuplicateEntity(EntityRef Ref)
{
	if (!Entities.contains(Ref))
	{
		CL_CORE_ERROR("Tried to duplicate a non-existent entity within the scene with RefID=%d", Ref);
		return { nullptr };
	}

	FEntityData& OriginalData = Entities[Ref];

	FEntity CloneEntityAb = CreateEntity(OriginalData.Name);
	FEntityData& CloneEntity = *CloneEntityAb.GetData();

	for (auto&& [TypeID, Component] : OriginalData.Components)
	{
		CloneEntity.Components[TypeID] =
		{
			TypeID,
			Component.Data->Clone()
		};
	}

	return CloneEntityAb;
}

void FScene::DestroyEntity(FEntity Entity)
{
	if (!Internal_ValidateEntityScene(Entity))
	{
		CL_CORE_ERROR("Tried to destroy an entity that didn't belong to that scene.", Entity.GetRef());
		return;
	}

	DestroyEntity(Entity.GetRef());
}

void FScene::DestroyEntity(EntityRef Ref)
{
	if (!Entities.contains(Ref))
	{
		CL_CORE_ERROR("Tried to destroy a non-existent entity within the scene with RefID=%d", Ref);
		return;
	}

	Entities[Ref].RemoveAllComponents();
	Entities.erase(Ref);
}

FEntity FScene::FindPrimaryCameraEntity()
{
	for (auto&& [RefID, Entity] : Entities)
	{
		if (Entity.HasComponent<FCameraComponent>())
		{
			FCameraComponent& CameraComponent = Entity.GetComponent<FCameraComponent>();

			if (CameraComponent.IsPrimary())
			{
				return { &Entity };
			}
		}
	}

	return { nullptr };
}

FEntity FScene::RetrieveFirstEntityByName(const FString& Name)
{
	for (auto&& [RefID, Entity] : Entities)
	{
		if (Entity.Name == Name)
		{
			return { &Entities.at(RefID) };
		}
	}

	return { nullptr };
}

TArray<FEntity> FScene::RetrieveEntitiesByName(const FString& Name)
{
	TArray<FEntity> RetrieveEntities;

	for (auto&& [RefID, Entity] : Entities)
	{
		if (Entity.Name == Name)
		{
			RetrieveEntities.Emplace(&Entity);
		}
	}

	return RetrieveEntities;
}

FEntity FScene::RetrieveEntity(EntityRef Ref)
{
	if (!Entities.contains(Ref))
	{
		CL_CORE_ERROR("Failed to retrieve entity with the RefID: %d, no entity exists on the Scene with that RefID.", Ref);
		return { nullptr };
	}

	FEntityData& Entity = Entities.at(Ref);
	return { &Entity };
}

FString FScene::GetEntityName(FEntity Entity)
{
	if (Internal_ValidateEntityScene(Entity))
	{
		return "";
	}

	return GetEntityName(Entity.GetRef());
}

FString FScene::GetEntityName(EntityRef Ref)
{
	if (!Entities.contains(Ref))
	{
		CL_CORE_ERROR("Failed to get the name of the entity with the RefID: %d, no entity exists on the Scene with that RefID.", Ref);
		return "";
	}

	return Entities.at(Ref).Name;
}

bool FScene::HasEntity(FEntity Entity)
{
	if (Entity.GetScene() != this)
	{
		return false;
	}

	return HasEntity(Entity.GetRef());
}

bool FScene::HasEntity(EntityRef Ref)
{
	return Entities.contains(Ref);
}

bool FScene::Internal_ValidateEntityScene(FEntity Entity)
{
	if (Entity.GetScene() != this)
	{
		CL_CORE_ERROR("Input entity (%s, %d) has a different Scene!", *GetEntityName(Entity), Entity.GetRef());
		return false;
	}

	return true;
}
