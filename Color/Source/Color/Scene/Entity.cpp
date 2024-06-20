#include "ColorPCH.h"
#include "Entity.h"

#include "Scene/Scene.h"

FEntityData* FEntityData::RetrieveData(EntityRef RefID, FScene* InScene)
{
	if (!InScene->Entities.contains(RefID))
	{
		CL_CORE_ERROR("Couldn't retrieve EntityData of Entity with RefID of %d. Specified Entity doesn't exist within the Scene.", RefID);
		return nullptr;
	}

	return &InScene->Entities.at(RefID);
}

FEntity::FEntity(EntityRef RefID, FScene* Scene)
	: RefID(RefID), Scene(Scene)
{
}

void FEntity::SetName(const FString& NewName)
{
	GetData()->Name = NewName;
}

FEntityData* FEntity::GetData() const
{
	return FEntityData::RetrieveData(RefID, Scene);
}

bool FEntity::IsValid() const
{
	return Scene != nullptr && GetData()->Scene->HasEntity(RefID);
}
