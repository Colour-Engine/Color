#include "ColorPCH.h"
#include "GlobalSerializationManager.h"

#include "Components/SpriteRendererComponent.h"
#include "Components/CircleRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

static THash<char*> Hasher;
static uint_t TC_HASH = Hasher.Hash(FTransformComponent::GetIDName());
static uint_t CC_HASH = Hasher.Hash(FCameraComponent::GetIDName());
static uint_t SRC_HASH = Hasher.Hash(FSpriteRendererComponent::GetIDName());
static uint_t CRC_HASH = Hasher.Hash(FCircleRendererComponent::GetIDName());

FArchive FGlobalSerializationManager::SerializeScene(FScene* Scene)
{
	FArchive Ar;
	FArchive& Entities = Ar.SetField("Entities", AFV_Group);

	for (auto&& [RefID, Entity] : Scene->GetEntities())
	{
		FArchive& EntityGroup = Entities.SetField(FString::FromInt((int32) Entity.RefID), FArchiveFieldValue(AFV_Group));
		EntityGroup.SetField("Name", Entity.Name);
		FArchive& ComponentsGroup = EntityGroup.SetField("Components", AFV_Group);

		for (auto&& [IDName, Component] : Entity.Components)
		{
			bool bSuccess;
			FArchive ComponentAr = SerializeComponent(Component.Data, bSuccess);

			if (!bSuccess)
			{
				CL_CORE_ERROR("Component with IDName '%s' that belongs to entity with the name '%s' wasn't serialized properly.", *IDName, *Entity.Name);
				continue;
			}

			FArchive& ComponentGroup = ComponentsGroup.SetField(Component.IDName, FArchiveFieldValue(AFV_Group));
			for (auto&& [K, V] : ComponentAr)
			{
				ComponentGroup.SetField(K, V);
			}
		}
	}

	return Ar;
}

bool FGlobalSerializationManager::DeserializeScene(FScene* Scene, const FArchive& Ar)
{
	// Make sure the Scene is empty.
	Scene->DestroyAllEntities();

	bool bResult = true;

	if (Ar.HasFieldWithType("Entities", AFV_Group))
	{
		const FArchive& Entities = Ar["Entities"];

		for (auto&& [EntityRefID, EntityDataValue] : Entities)
		{
			verifyf(EntityDataValue.GetType() == AFV_Group, "EntityData must be of type 'Group'!");
			const FArchive& EntityData = EntityDataValue;

			FString EntityName = "";
			GetFieldChecked(EntityData, "Name", String, EntityName, bResult);

			FEntity Entity = Scene->CreateEntity(EntityName);
			if (!bResult)
			{
				CL_CORE_ERROR("No string 'Name' property was found for Entity (SerialData RefID=%d)", EntityRefID);
			}

			const FArchive& Components = EntityData["Components"];
			for (auto&& [ComponentIDName, ComponentDataValue] : Components)
			{
				verifyf(ComponentDataValue.GetType() == AFV_Group, "ComponentData must be of type 'Group'!");
				const FArchive& ComponentData = ComponentDataValue;

				bResult = DeserializeComponent(*ComponentIDName, ComponentData, Entity);
				if (!bResult)
				{
					CL_CORE_ERROR("Component '%s' might've not been deserialized correctly! (OwnerEntity: { RefID: %d, Name: %s })", *ComponentIDName, Entity.GetRef(), *Entity.GetName());
				}
			}
		}
	}

	return bResult;
}

FArchive FGlobalSerializationManager::SerializeComponent(FComponent* Component, bool& bOutSuccess)
{
	FArchive Ar;

	uint_t IDNameHash = Hasher.Hash(Component->GetIdentificationName());
	if (IDNameHash == TC_HASH)
	{
		FTransformComponent* TransformComponent = Cast<FTransformComponent>(Component);

		FArchiveHelpers::SetVec3Field(Ar, "Location", TransformComponent->GetLocation());
		FArchiveHelpers::SetVec3Field(Ar, "Rotation", TransformComponent->GetRotation());
		FArchiveHelpers::SetVec3Field(Ar, "Scale", TransformComponent->GetScale());
	}
	else if (IDNameHash == CC_HASH)
	{
		FCameraComponent* CameraComponent = Cast<FCameraComponent>(Component);

		Ar.SetField("Camera", CameraComponent->GetCamera().Serialize());
		Ar.SetField("bPrimary", CameraComponent->IsPrimary());
		Ar.SetField("bFixedAspectRatio", CameraComponent->HasFixedAspectRatio());
	}
	else if (IDNameHash == SRC_HASH)
	{
		FSpriteRendererComponent* SpriteRendererComponent = Cast<FSpriteRendererComponent>(Component);

		Ar.SetField("TexturePath", SpriteRendererComponent->GetTexture()->GetPath());
		Ar.SetField("TilingFactor", SpriteRendererComponent->GetTilingFactor());
		FArchiveHelpers::SetVec4Field(Ar, "Color", SpriteRendererComponent->GetColor());
	}
	else if (IDNameHash == CRC_HASH)
	{
		FCircleRendererComponent* CircleRendererComponent = Cast<FCircleRendererComponent>(Component);

		FArchiveHelpers::SetVec4Field(Ar, "Color", CircleRendererComponent->GetColor());
		Ar.SetField("Thickness", CircleRendererComponent->GetThickness());
		Ar.SetField("Fade", CircleRendererComponent->GetFade());
	}
	else
	{
		return SerializeUnrecognizedComponent(Component, bOutSuccess);
	}

	return Ar;
}

bool FGlobalSerializationManager::DeserializeComponent(const char* IDName, const FArchive& Ar, FEntity Entity)
{
	bool bSuccess = true;
	uint_t IDNameHash = Hasher.Hash(IDName);

	if (IDNameHash == TC_HASH)
	{
		FTransformComponent& TransformComponent = Entity.GetOrAddComponent<FTransformComponent>();

		glm::vec3 Location, Rotation, Scale;
		GetVecFieldChecked(Ar, "Location", Vec3, Location, bSuccess);
		GetVecFieldChecked(Ar, "Rotation", Vec3, Rotation, bSuccess);
		GetVecFieldChecked(Ar, "Scale", Vec3, Scale, bSuccess);

		TransformComponent.SetLocation(Location);
		TransformComponent.SetRotation(Rotation);
		TransformComponent.SetScale(Scale);
	}
	else if (IDNameHash == CC_HASH)
	{
		FCameraComponent& CameraComponent = Entity.GetOrAddComponent<FCameraComponent>();

		if (Ar.HasFieldWithType("Camera", AFV_Group))
		{
			CameraComponent.GetCamera().Deserialize(Ar["Camera"]);
		}
		else
		{
			CL_CORE_ERROR("CameraData couldn't be found for CameraComponent deserialization!");
		}

		bool bPrimary=false, bFixedAspectRatio=false;
		GetFieldChecked(Ar, "bPrimary", Bool, bPrimary, bSuccess);
		GetFieldChecked(Ar, "bFixedAspectRatio", Bool, bFixedAspectRatio, bSuccess);

		CameraComponent.SetIsPrimary(bPrimary);
		CameraComponent.SetUseFixedAspectRatio(bFixedAspectRatio);
	}
	else if (IDNameHash == SRC_HASH)
	{
		FSpriteRendererComponent& SpriteRendererComponent = Entity.GetOrAddComponent<FSpriteRendererComponent>();

		FString TexturePath;
		glm::vec4 Color;
		float TilingFactor=.0f;

		GetFieldChecked(Ar, "TexturePath", String, TexturePath, bSuccess);
		GetFieldChecked(Ar, "TilingFactor", Float, TilingFactor, bSuccess);
		GetVecFieldChecked(Ar, "Color", Vec4, Color, bSuccess);

		SpriteRendererComponent.SetTexture(FTexture2D::New(TexturePath));
		SpriteRendererComponent.SetTilingFactor(TilingFactor);
		SpriteRendererComponent.SetColor(Color);
	}
	else if (IDNameHash == CRC_HASH)
	{
		FCircleRendererComponent& CircleRendererComponent = Entity.GetOrAddComponent<FCircleRendererComponent>();

		glm::vec4 Color;
		float Thickness=.0f,Fade=.0f;

		GetVecFieldChecked(Ar, "Color", Vec4, Color, bSuccess);
		GetFieldChecked(Ar, "Thickness", Float, Thickness, bSuccess);
		GetFieldChecked(Ar, "Fade", Float, Fade, bSuccess);

		CircleRendererComponent.SetColor(Color);
		CircleRendererComponent.SetThickness(Thickness);
		CircleRendererComponent.SetFade(Fade);
	}
	else
	{
		return DeserializeUnrecognizedComponent(IDName, Ar, Entity);
	}

	return bSuccess;
}

FArchive FGlobalSerializationManager::SerializeUnrecognizedComponent(FComponent* Component, bool& bOutSuccess)
{
	bOutSuccess = false;
	return FArchive();
}

bool FGlobalSerializationManager::DeserializeUnrecognizedComponent(const char* IDName, const FArchive& Ar, FEntity Entity)
{
	return false;
}
