#pragma once

#include "Core/Base.h"
#include "Scene/Entity.h"

class FScene
{
public:
	FScene();
	~FScene();

	void Start();
	void Puase();
	void Unpause();
	void SetPaused(bool bPaused);
	void Stop();

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

	void DestroyAllEntities();
	void DestroyEntity(FEntity Entity);
	void DestroyEntity(EntityRef Ref);

	FEntity FindPrimaryCameraEntity();
	FEntity RetrieveFirstEntityByName(const FString& Name);
	TArray<FEntity> RetrieveEntitiesByName(const FString& Name);
	FEntity RetrieveEntity(EntityRef Ref);

	FString GetEntityName(FEntity Entity);
	FString GetEntityName(EntityRef Ref);

	bool HasEntity(FEntity Entity);
	bool HasEntity(EntityRef Ref);

	bool IsRunning() const { return bRunning; }
	bool IsPaused() const { return bPaused; }
	
	std::map<EntityRef, FEntityData>& GetEntities() { return Entities; }
private:
	bool Internal_ValidateEntityScene(FEntity Ref);
private:
	std::map<EntityRef, FEntityData> Entities;

	bool bRunning = false;
	bool bPaused = false;
private:
	friend struct FEntityData;
};
