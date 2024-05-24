#pragma once

#include "Core/Base.h"
#include "Scene/Entity.h"

class FScene
{
public:
	FScene();
	~FScene();

	void TickScene(float DeltaTime);

	FEntity CreateEntityWithRefID(EntityRef RefID, const FString& Name = "Unnamed Entity");
	FEntity CreateEntity(const FString& Name = "Unnamed Entity");
	
	/**
	 * !!!DISCLAMER!!!
	 * To have proper duplicating entites, each component that entity has must properly override and implement the Clone() method.
	 * The CLONEINIT() and CLONEFINISH macros are really useful for the Clone() method, if you're trying to support it.
	 */
	FEntity DuplicateEntity(FEntity Entity);

	/**
	 * !!!DISCLAMER!!!
	 * To have proper duplicating entites, each component that entity has must properly override and implement the Clone() method.
	 * The CLONEINIT() and CLONEFINISH macros are really useful for the Clone() method, if you're trying to support it.
	 */
	FEntity DuplicateEntity(EntityRef Ref);

	void DestroyEntity(FEntity Entity);
	void DestroyEntity(EntityRef Ref);

	FEntity RetrieveFirstEntityByName(const FString& Name);
	TArray<FEntity> RetrieveEntitiesByName(const FString& Name);
	FEntity RetrieveEntity(EntityRef Ref);

	FString GetEntityName(FEntity Entity);
	FString GetEntityName(EntityRef Ref);

	bool HasEntity(FEntity Entity);
	bool HasEntity(EntityRef Ref);
private:
	bool Internal_ValidateEntityScene(FEntity Ref);
private:
	std::unordered_map<EntityRef, FEntityData> Entities;
private:
	friend struct FEntityData;
};
