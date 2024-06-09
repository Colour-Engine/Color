#include "Sandbox2D.h"

#include "Core/Application.h"
#include "Input/Input.h"

#include "Components/SpriteRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"

#include "Utils/FileSystem.h"

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

	// Serialize the scene
	FArchive SceneData = GetGlobalSerializationManager()->SerializeScene(Scene.Get());

	// Delete all entities and components
	Scene->DestroyAllEntities();

	// Reload all saved data via deserialization
	GetGlobalSerializationManager()->DeserializeScene(Scene.Get(), SceneData);

	// Save the scene data to disk to load later
	FFileSystem::CreateDirectories("Cache/SerialData");
	FFileSystem::WriteToFile("Cache/SerialData/SceneData.clarf", *SceneData.ConvertToCLARF());
}

void FSandbox2D::OnTick(float DeltaTime)
{
	FEntity CameraEntity = Scene->FindPrimaryCameraEntity();

	if (FInput::IsKeyPressed(Key::V) || FInput::IsMouseButtonPressed(Mouse::ButtonLeft))
	{
		FRenderCommand::SetRenderMode(ERenderMode::Wireframe);
	}
	else
	{
		FRenderCommand::SetRenderMode(ERenderMode::Solid);
	}

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
