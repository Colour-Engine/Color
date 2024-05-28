#pragma once

#include "Scene/Component.h"

#include "Renderer/Texture.h"

#include <glm/glm.hpp>

class FSpriteRendererComponent : public FComponent
{
public:
	FSpriteRendererComponent() = default;
	FSpriteRendererComponent(const TRef<FTexture2D>& Texture, const glm::vec4& Color = { 1.0f, 1.0f, 1.0f, 1.0f }, float TilingFactor = 1.0f);
	FSpriteRendererComponent(const TRef<FTexture2D>& Texture, float TilingFactor = 1.0f, const glm::vec4& Color = { 1.0f, 1.0f, 1.0f, 1.0f });
	FSpriteRendererComponent(const glm::vec4& Color);

	virtual FArchive Serialize() const override;
	virtual bool Deserialize(const FArchive& Archive) override;
	virtual FComponent* Clone() const override;
	virtual void OnTick(float DeltaTime) override;

	void SetTexture(const TRef<FTexture2D>& Texture);
	void SetColor(const glm::vec4& Color);
	void SetTilingFactor(float TilingFactor);

	const TRef<FTexture2D>& GetTexture() const { return Texture; }
	const glm::vec4& GetColor() const { return Color; }
	float GetTilingFactor() const { return TilingFactor; }
private:
	TRef<FTexture2D> Texture;
	glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float TilingFactor = 1.0f;
};
