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

FEntity::FEntity(FEntityData* InData)
	: Data(InData)
{
}
