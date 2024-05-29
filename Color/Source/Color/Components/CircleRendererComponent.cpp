#include "ColorPCH.h"
#include "CircleRendererComponent.h"

#include "Components/TransformComponent.h"

#include "Renderer/Renderer2D.h"
#include "Scene/Scene.h"

FCircleRendererComponent::FCircleRendererComponent(const glm::vec4& Color, float Thickness, float Fade)
	: Color(Color), Thickness(Thickness), Fade(Fade)
{
}

FComponent* FCircleRendererComponent::Clone() const
{
	CLONESTART(FCircleRendererComponent, FComponent);

	Cloned->Color = Color;
	Cloned->Thickness = Thickness;
	Cloned->Fade = Fade;

	CLONEFINISH;
}

void FCircleRendererComponent::OnTick(float DeltaTime)
{
	FComponent::OnTick(DeltaTime);
	
	FEntity Owner = GetOwnerScene()->RetrieveEntity(GetOwnerRef());
	if (Owner.HasComponent<FTransformComponent>())
	{
		FTransformComponent& TransformComponent = Owner.GetComponent<FTransformComponent>();

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), TransformComponent.GetLocation()) *
						      glm::rotate(glm::mat4(1.0f), glm::radians(TransformComponent.GetRotation2D()), { 0.0f, 0.0f, 1.0f }) *
						      glm::scale(glm::mat4(1.0f), { TransformComponent.GetScale2D().x, TransformComponent.GetScale2D().y, 1.0f });

		FRenderer2D::DrawCircle(Transform, *this, (int) GetOwnerRef());
	}
}

void FCircleRendererComponent::SetColor(const glm::vec4& NewColor)
{
	Color = NewColor;
}

void FCircleRendererComponent::SetThickness(float NewThickness)
{
	Thickness = NewThickness;
}

void FCircleRendererComponent::SetFade(float NewFade)
{
	Fade = NewFade;
}
