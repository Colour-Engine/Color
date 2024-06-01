#pragma once

#include "Scene/Component.h"

#include <glm/glm.hpp>

class FCircleRendererComponent : public FComponent
{
public:
	static const char* GetIDName() { return "CircleRendererComponent"; }
public:
	FCircleRendererComponent() = default;
	FCircleRendererComponent(const glm::vec4& Color, float Thickness = 1.0f, float Fade = 0.005f);

	virtual void OnTick(float DeltaTime) override;
	virtual FComponent* Clone() const override;

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
