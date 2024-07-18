#include "ColorPCH.h"
#include "Scene.h"

#include "Components/SpriteRendererComponent.h"
#include "Components/CircleRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

#include "Asset/Importers/TextureImporter.h"
#include "Project/Project.h"

static THash<char*> Hasher;
static uint_t TC_HASH = Hasher.Hash(FTransformComponent::GetIDName());
static uint_t CC_HASH = Hasher.Hash(FCameraComponent::GetIDName());
static uint_t SRC_HASH = Hasher.Hash(FSpriteRendererComponent::GetIDName());
static uint_t CRC_HASH = Hasher.Hash(FCircleRendererComponent::GetIDName());

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

void FScene::Start()
{
	bRunning = true;
}

void FScene::Puase()
{
	bPaused = true;
}

void FScene::Unpause()
{
	bPaused = false;
}

void FScene::SetPaused(bool bPaused)
{
	this->bPaused = bPaused;
}

void FScene::Stop()
{
	bRunning = false;
}

void FScene::TickScene(float DeltaTime)
{
	if (bRunning)
	{
		for (auto&& [Ref, Entity] : Entities)
		{
			for (auto&& [TypeID, Component] : Entity.Components)
			{
				if (Component.ComponentObject->IsTickEnabled() && (!bPaused || Component.ComponentObject->DoesTickWhenPaused()))
				{
					Component.ComponentObject->OnTick(DeltaTime);
				}
			}
		}
	}
}

FEntity FScene::CreateEntityWithRefID(EntityRef RefID, const FString& Name)
{
	if (Entities.contains(RefID))
	{
		CL_CORE_ERROR("Failed to create entity '%s' with the RefID: %d, another entity with that ID already exists.", *Name, RefID);
		return { };
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

	return { RefID, this };
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
		return { };
	}

	return DuplicateEntity(Entity.GetRef());
}

FEntity FScene::DuplicateEntity(EntityRef Ref)
{
	if (!Entities.contains(Ref))
	{
		CL_CORE_ERROR("Tried to duplicate a non-existent entity within the scene with RefID=%d", Ref);
		return { };
	}

	FEntityData& OriginalData = Entities[Ref];

	FEntity CloneEntityAb = CreateEntity(OriginalData.Name);
	FEntityData& CloneEntity = *CloneEntityAb.GetData();

	for (auto&& [TypeID, Component] : OriginalData.Components)
	{
		CloneEntity.Components[TypeID] =
		{
			TypeID,
			Component.ComponentObject->Clone()
		};
	}

	return CloneEntityAb;
}

void FScene::DestroyAllEntities()
{
	while (!Entities.empty())
	{
		DestroyEntity((*Entities.begin()).first);
	}
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
				return { RefID, this };
			}
		}
	}

	return { };
}

FEntity FScene::RetrieveFirstEntityByName(const FString& Name)
{
	for (auto&& [RefID, Entity] : Entities)
	{
		if (Entity.Name == Name)
		{
			return { RefID, this };
		}
	}

	return { };
}

TArray<FEntity> FScene::RetrieveEntitiesByName(const FString& Name)
{
	TArray<FEntity> RetrieveEntities;

	for (auto&& [RefID, Entity] : Entities)
	{
		if (Entity.Name == Name)
		{
			RetrieveEntities.Emplace(RefID, this);
		}
	}

	return RetrieveEntities;
}

FEntity FScene::RetrieveEntity(EntityRef Ref)
{
	if (!Entities.contains(Ref))
	{
		CL_CORE_ERROR("Failed to retrieve entity with the RefID: %d, no entity exists on the Scene with that RefID.", Ref);
		return { };
	}

	return { Ref, this };
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

FArchive FScene::Serialize() const
{
	FArchive Ar;

	Ar.SetField("bRunning", bRunning);
	Ar.SetField("bPaused", bPaused);

	FArchive& Entities = Ar.SetField("Entities", AFV_Group);
	for (auto&& [RefID, Entity] : this->Entities)
	{
		FArchive& EntityGroup = Entities.SetField(FString::FromInt(RefID), FArchiveFieldValue(AFV_Group));
		EntityGroup.SetField("Name", Entity.Name);
		FArchive& ComponentsGroup = EntityGroup.SetField("Components", AFV_Group);

		for (auto&& [IDName, Component] : Entity.Components)
		{
			bool bSuccess;
			auto SerializeComponent = [&bSuccess](FComponent* Component) -> FArchive
			{
				FArchive Ar;

				Ar.SetField("bEnableTick", Component->IsTickEnabled());
				Ar.SetField("bTickWhenPaused", Component->DoesTickWhenPaused());

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
					Ar.SetField("TextureAssetHandle", SpriteRendererComponent->GetTexture()->AssetHandle);
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
					CL_CORE_ERROR("Failed to serialize an unrecognized component! IDName '%s'", Component->GetIdentificationName());
				}

				return Ar;
			};

			FArchive ComponentAr = SerializeComponent(Component.ComponentObject);
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

bool FScene::Deserialize(const FArchive& Ar)
{
	// Make sure the Scene is empty.
	DestroyAllEntities();

	bool bResult = true;
	bool bRunning, bPaused;

	GetFieldChecked(Ar, "bRunning", Bool, bRunning, bResult);
	GetFieldChecked(Ar, "bPaused", Bool, bPaused, bResult);

	if (bRunning)
	{
		Start();
	}
	else
	{
		Stop();
	}
	SetPaused(bPaused);

	if (Ar.HasFieldWithType("Entities", AFV_Group))
	{
		const FArchive& Entities = Ar["Entities"];

		for (auto&& [EntityRefID, EntityDataValue] : Entities)
		{
			verifyf(EntityDataValue.GetType() == AFV_Group, "EntityData must be of type 'Group'!");
			const FArchive& EntityData = EntityDataValue;

			FString EntityName = "";
			GetFieldChecked(EntityData, "Name", String, EntityName, bResult);

			FEntity Entity = CreateEntityWithRefID(EntityRefID.ToInteger(), EntityName);
			if (!bResult)
			{
				CL_CORE_ERROR("No string 'Name' property was found for Entity (SerialData RefID=%d)", EntityRefID);
			}

			const FArchive& Components = EntityData["Components"];
			for (auto&& [ComponentIDName, ComponentDataValue] : Components)
			{
				verifyf(ComponentDataValue.GetType() == AFV_Group, "ComponentData must be of type 'Group'!");
				const FArchive& ComponentData = ComponentDataValue;

				auto DeserializeComponent = [](FStringView IDName, const FArchive& Ar, FEntity Entity) -> bool
				{
					#define DeserializeComponentStart(Component) { bool bFlag = false; \
					bool bEnableTick = true; \
					bool bTickWhenPaused = false; \
					GetFieldChecked(Ar, "bEnableTick", Bool, bEnableTick, bFlag); \
					GetFieldChecked(Ar, "bTickWhenPaused", Bool, bTickWhenPaused, bFlag); \
					Component.SetTickRules(bEnableTick, bTickWhenPaused); \
					}

					bool bSuccess = true;
					uint_t IDNameHash = Hasher.Hash(*IDName);

					bool bEnableTick = true;
					bool bTickWhenPaused = false;

					if (IDNameHash == TC_HASH)
					{
						FTransformComponent& TransformComponent = Entity.GetOrAddComponent<FTransformComponent>();
						DeserializeComponentStart(TransformComponent);

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
						DeserializeComponentStart(CameraComponent);

						if (Ar.HasFieldWithType("Camera", AFV_Group))
						{
							CameraComponent.GetCamera().Deserialize(Ar["Camera"]);
						}
						else
						{
							CL_CORE_ERROR("CameraData couldn't be found for CameraComponent deserialization!");
						}

						bool bPrimary = false, bFixedAspectRatio = false;
						GetFieldChecked(Ar, "bPrimary", Bool, bPrimary, bSuccess);
						GetFieldChecked(Ar, "bFixedAspectRatio", Bool, bFixedAspectRatio, bSuccess);

						CameraComponent.SetIsPrimary(bPrimary);
						CameraComponent.SetUseFixedAspectRatio(bFixedAspectRatio);
					}
					else if (IDNameHash == SRC_HASH)
					{
						FSpriteRendererComponent& SpriteRendererComponent = Entity.GetOrAddComponent<FSpriteRendererComponent>();
						DeserializeComponentStart(SpriteRendererComponent);

						FString TexturePath;
						HAsset TextureAssetHandle = HASSET_INVALID_HANDLE;
						glm::vec4 Color;
						float TilingFactor = .0f;

						GetFieldChecked(Ar, "TexturePath", String, TexturePath, bSuccess);
						GetFieldChecked(Ar, "TextureAssetHandle", UInteger, TextureAssetHandle, bSuccess);
						GetFieldChecked(Ar, "TilingFactor", Float, TilingFactor, bSuccess);
						GetVecFieldChecked(Ar, "Color", Vec4, Color, bSuccess);

						if (TextureAssetHandle != HASSET_INVALID_HANDLE)
						{
							TRef<FAsset> Asset = FProject::GetActiveAssetManager()->GetAsset(TextureAssetHandle);
							FTexture2D* TextureAsset = Cast<FTexture2D>(Asset.Get());

							if (!TextureAsset)
							{
								CL_CORE_ERROR("Deserializing failure during SpriteRendererComponent deserialization! TextureAssetHandle != HASSET_INVALID_HANDLE but ActiveAssetManager returned nullptr when the asset with the assigned handle '%e' was requested. Either the asset does not exist or it corresponds to a different type of asset.");
							}

							SpriteRendererComponent.SetTexture(Asset);
						}
						else
						{
							SpriteRendererComponent.SetTexture(FTextureImporter::LoadTexture2D(TexturePath));
						}

						SpriteRendererComponent.SetTilingFactor(TilingFactor);
						SpriteRendererComponent.SetColor(Color);
					}
					else if (IDNameHash == CRC_HASH)
					{
						FCircleRendererComponent& CircleRendererComponent = Entity.GetOrAddComponent<FCircleRendererComponent>();
						DeserializeComponentStart(CircleRendererComponent);

						glm::vec4 Color;
						float Thickness = .0f, Fade = .0f;

						GetVecFieldChecked(Ar, "Color", Vec4, Color, bSuccess);
						GetFieldChecked(Ar, "Thickness", Float, Thickness, bSuccess);
						GetFieldChecked(Ar, "Fade", Float, Fade, bSuccess);

						CircleRendererComponent.SetColor(Color);
						CircleRendererComponent.SetThickness(Thickness);
						CircleRendererComponent.SetFade(Fade);
					}
					else
					{
						CL_CORE_ERROR("Failed to deserialize an unrecognized component! IDName '%s'", *IDName);
						return false;
					}

					return bSuccess;
				#undef DeserializeComponentStart
				};

				bResult = DeserializeComponent(ComponentIDName, ComponentData, Entity);
				if (!bResult)
				{
					CL_CORE_ERROR("Component '%s' might've not been deserialized correctly! (OwnerEntity: { RefID: %d, Name: %s })", *ComponentIDName, Entity.GetRef(), *Entity.GetName());
				}
			}
		}
	}

	return bResult;
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
