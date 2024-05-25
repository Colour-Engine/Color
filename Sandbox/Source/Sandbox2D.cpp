#include "Sandbox2D.h"

#include "Core/Application.h"

#include "Components/SpriteRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"

void FSandbox2D::OnAttach()
{
	Scene = MakeScope<FScene>();

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
}

void FSandbox2D::OnTick(float DeltaTime)
{
	FEntity CameraEntity = Scene->FindPrimaryCameraEntity();

	FRenderCommand::Clear();
	FRenderer2D::BeginScene(CameraEntity.GetComponent<FCameraComponent>().GetCamera(), CameraEntity.GetComponent<FTransformComponent>().GetTransformMatrix());

	Scene->TickScene(DeltaTime);

	FRenderer2D::EndScene();
}

void FSandbox2D::OnDetach()
{
}
