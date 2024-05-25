#pragma once

#include "Core/Base.h"

#include <glm/glm.hpp>

class FCamera
{
public:
	FCamera() = default;
	FCamera(const glm::mat4& InProjection);
	virtual ~FCamera() = default;

	const glm::mat4& GetProjection() const { return Projection; }
protected:
	glm::mat4 Projection = glm::mat4(1.0f);
};
