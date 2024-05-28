#pragma once

#include "Scene/Component.h"

#include <glm/glm.hpp>

class FCircleRendererComponent : public FComponent
{
public:
	FCircleRendererComponent() = default;
	FCircleRendererComponent(const glm::vec4& Color, float Thickness = 1.0f, float Fade = 0.005f);

	virtual FArchive Serialize() const override;
	virtual bool Deserialize(const FArchive& Archive) override;
	virtual FComponent* Clone() const override;
	virtual void OnTick(float DeltaTime) override;

	void SetColor(const glm::vec4& NewColor);
	void SetThickness(float NewThickness);
	void SetFade(float NewFade);

	const glm::vec4& GetColor() const { return Color; }
	float GetThickness() const { return Thickness; }
	float GetFade() const { return Fade; }
private:
	glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float Thickness = 1.0f;
	float Fade = 0.005f;
};
