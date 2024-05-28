#include "ColorPCH.h"
#include "SpriteRendererComponent.h"

#include "Components/TransformComponent.h"

#include "Renderer/Renderer2D.h"
#include "Scene/Scene.h"

#include <glm/gtc/matrix_transform.hpp>

FSpriteRendererComponent::FSpriteRendererComponent(const TRef<FTexture2D>& Texture, const glm::vec4& Color, float TilingFactor)
	: Texture(Texture), Color(Color), TilingFactor(TilingFactor)
{
}

FSpriteRendererComponent::FSpriteRendererComponent(const TRef<FTexture2D>& Texture, float TilingFactor, const glm::vec4& Color)
	: Texture(Texture), TilingFactor(TilingFactor), Color(Color)
{
}

FSpriteRendererComponent::FSpriteRendererComponent(const glm::vec4& Color)
	: Color(Color)
{
}

FArchive FSpriteRendererComponent::Serialize() const
{
	SERIALIZESTART();

	SerializeAr.SetField("TexturePath", Texture->GetPath());
	FArchiveHelpers::SetVec4Field(SerializeAr, "Color", Color);
	SerializeAr.SetField("TilingFactor", TilingFactor);

	SERIALIZEFINISH;
}

bool FSpriteRendererComponent::Deserialize(const FArchive& Archive)
{
	DESERIALIZESTART();
	
	FString TexturePath;
	GetFieldChecked("TexturePath", String, TexturePath);
	GetVecFieldChecked("Color", Vec4, Color);
	GetFieldChecked("TilingFactor", Float, TilingFactor);

	Texture = FTexture2D::New(TexturePath);

	DESERIALIZEFINISH;
}

FComponent* FSpriteRendererComponent::Clone() const
{
	CLONESTART(FSpriteRendererComponent, FComponent);

	Cloned->Texture = Texture;
	Cloned->TilingFactor = TilingFactor;
	Cloned->Color = Color;

	CLONEFINISH;
}

void FSpriteRendererComponent::OnTick(float DeltaTime)
{
	FComponent::OnTick(DeltaTime);
	
	FEntity Owner = GetOwnerScene()->RetrieveEntity(GetOwnerRef());
	if (Owner.HasComponent<FTransformComponent>())
	{
		FTransformComponent& TransformComponent = Owner.GetComponent<FTransformComponent>();

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), TransformComponent.GetLocation()) *
						      glm::rotate(glm::mat4(1.0f), glm::radians(TransformComponent.GetRotation2D()), { 0.0f, 0.0f, 1.0f }) *
						      glm::scale(glm::mat4(1.0f), { TransformComponent.GetScale2D().x, TransformComponent.GetScale2D().y, 1.0f });

		FRenderer2D::DrawSprite(Transform, *this, (int) GetOwnerRef());
	}
}

void FSpriteRendererComponent::SetTexture(const TRef<FTexture2D>& Texture)
{
	this->Texture = Texture;
}

void FSpriteRendererComponent::SetColor(const glm::vec4& Color)
{
	this->Color = Color;
}

void FSpriteRendererComponent::SetTilingFactor(float TilingFactor)
{
	this->TilingFactor = TilingFactor;
}
