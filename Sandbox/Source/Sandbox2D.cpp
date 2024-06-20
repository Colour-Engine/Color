#include "Sandbox2D.h"

#include "Core/Application.h"
#include "Input/Input.h"

#include "Components/SpriteRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"

#include "Utils/FileSystem.h"
#include "Utils/CLARF.h"

void FSandbox2D::OnAttach()
{
	Scene = MakeRef<FScene>();
	Scene->Start();

	FEntity CamEntity = Scene->CreateEntity("MainCamera");
	FCameraComponent& CameraComponent = CamEntity.AddComponent<FCameraComponent>();

	CameraComponent.GetCamera().SetViewportSize(FApplication::Get()->GetWindow()->GetWidth(), FApplication::Get()->GetWindow()->GetHeight());
	CameraComponent.SetIsPrimary(true);

	for (int i = 0; i < 2; i++)
	{
		FEntity QuadEntity = Scene->CreateEntity(FString::Format("QuadEntity_%d", i));
		FSpriteRendererComponent& SpriteRendererComponent = QuadEntity.AddComponent<FSpriteRendererComponent>();
		FTransformComponent& TransformComponent = QuadEntity.GetComponent<FTransformComponent>();

		if (i == 0)
		{
			SpriteRendererComponent.SetTexture(FTexture2D::New("Content/Textures/Engine_Logo_Wide_NoBG.png"));
			TransformComponent.SetScale2D({ 10.0f, 6.0f });
		}
		else if (i == 1)
		{
			CrescentEntity = QuadEntity;
			SpriteRendererComponent.SetTexture(FTexture2D::New("Content/Textures/Turkish_Star_and_Crescent.png"));
			TransformComponent.SetLocation2D({ -6.0f, 3.0f });
			TransformComponent.SetRotation2D(-25.0f);
			TransformComponent.SetScale2D({ 5.0f, 5.0f });
		}
	}

	FEntity CircleEntity = Scene->CreateEntity("CircleEntity");
	FCircleRendererComponent& CircleRendererComponent = CircleEntity.AddComponent<FCircleRendererComponent>();
	FTransformComponent& TransformComponent = CircleEntity.GetComponent<FTransformComponent>();

	CircleRendererComponent.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	TransformComponent.SetLocation({ 6.0f, -3.0f });
	TransformComponent.SetScale2D({ 2.0f, 2.0f });

	{
		// Serialize the scene
		FArchive SceneData = GetGlobalSerializationManager()->SerializeScene(Scene.Get());
		FString  CLARFData = SceneData.ConvertToCLARF();
	
		// Delete all entities and components
		Scene->DestroyAllEntities();
	
		// Save the scene data to disk to load later
		FFileSystem::CreateDirectories("Cache/SerialData");
		FFileSystem::WriteToFile("Cache/SerialData/SceneData.clarf", CLARFData);
	}
	
	FString CLARFData;
	FFileSystem::ReadFile("Cache/SerialData/SceneData.clarf", CLARFData);
	
	FArchive SceneData = FCLARF::Load(CLARFData);
	GetGlobalSerializationManager()->DeserializeScene(Scene.Get(), SceneData);

	// Unnecessary because FGlobalDeserializationManager::DeserializeScene loads the entites with the same RefID they were serialized with.
	// Before the commit that added this "deserialize with the same RefID" feature, the code line below would be necessary.
	CrescentEntity = Scene->RetrieveFirstEntityByName("QuadEntity_1");
}

void FSandbox2D::OnTick(float DeltaTime)
{
	FEntity CameraEntity = Scene->FindPrimaryCameraEntity();

	if (FInput::IsKeyPressed(Key::V))
	{
		FRenderCommand::SetRenderMode(ERenderMode::Wireframe);
	}
	else
	{
		FRenderCommand::SetRenderMode(ERenderMode::Solid);
	}

	const float Speed = 5.0f;
	const float RotFactor = 250.0f;

	const bool bUpKeyDown = FInput::IsKeyPressed(Key::W);
	const bool bDownKeyDown = FInput::IsKeyPressed(Key::S);
	const bool bLeftKeyDown = FInput::IsKeyPressed(Key::A);
	const bool bRightKeyDown = FInput::IsKeyPressed(Key::D);

	const bool bLeftArrowDown = FInput::IsKeyPressed(Key::Left);
	const bool bRightArrowDown = FInput::IsKeyPressed(Key::Right);

	float VerticalMovement = (((bUpKeyDown && bDownKeyDown) || (!bUpKeyDown && !bDownKeyDown)) ? 0.0f : bUpKeyDown ? 1.0f : bDownKeyDown ? -1.0f : 0.0f) * Speed;
	float HorizontalMovement = (((bLeftKeyDown && bRightKeyDown) || (!bLeftKeyDown && !bRightKeyDown)) ? 0.0f : bLeftKeyDown ? -1.0f : bRightKeyDown ? 1.0f : 0.0f) * Speed;

	FTransformComponent& TC = CrescentEntity.GetTransform();
	TC.SetLocation2D(TC.GetLocation2D() + glm::vec2 { HorizontalMovement, VerticalMovement } * DeltaTime);

	float TickRotation = 0.0f;
	if (bLeftArrowDown)
	{
		TickRotation += RotFactor;
	}
	if (bRightArrowDown)
	{
		TickRotation += -RotFactor;
	}
	TC.SetRotation2D(TC.GetRotation2D() + TickRotation * DeltaTime);

	FRenderCommand::Clear();
	if (CameraEntity.IsValid())
	{
		FRenderer2D::BeginScene(CameraEntity.GetComponent<FCameraComponent>().GetCamera(), CameraEntity.GetComponent<FTransformComponent>().GetTransformMatrix());
	}
	Scene->TickScene(DeltaTime);
	if (CameraEntity.IsValid())
	{
		FRenderer2D::EndScene();
	}
	else
	{
		FRenderer2D::Discard();
	}
}

void FSandbox2D::OnDetach()
{
}
