#pragma once

#include "Core/Base.h"
#include "Core/Archive.h"

#include <glm/glm.hpp>

class FCamera
{
public:
	FCamera() = default;
	FCamera(const glm::mat4& InProjection);
	virtual ~FCamera() = default;

	virtual FArchive Serialize() const;
	virtual bool Deserialize(const FArchive& Archive);

	const glm::mat4& GetProjection() const { return Projection; }
protected:
	glm::mat4 Projection = glm::mat4(1.0f);
};
