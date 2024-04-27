#include "ColorPCH.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

FTransform::FTransform()
{
	RecalculateTransformMatrix();
}

FTransform::FTransform(const glm::vec2& NewLocation, float NewRotation, const glm::vec2& NewScale)
{
	SetAll(NewLocation, NewRotation, NewScale);
}

FTransform::FTransform(const glm::vec3& NewLocation, const glm::vec3& NewRotation, const glm::vec3& NewScale)
{
	SetAll(NewLocation, NewRotation, NewScale);
}

// See the definition of this constructor variant to understand why it's commented out.
// FTransform::FTransform(const glm::vec2& NewLocation, float NewRotation)
// {
// 	SetLocationAndRotation(NewLocation, NewRotation);
// }

FTransform::FTransform(const glm::vec2& NewLocation, const glm::vec2& NewScale)
{
	SetLocationAndScale(NewLocation, NewScale);
}

FTransform::FTransform(float NewRotation, const glm::vec2& NewScale)
{
	SetRotationAndScale(NewRotation, NewScale);
}

void FTransform::SetAll2D(const glm::vec2& NewLocation, float NewRotation, const glm::vec2& NewScale)
{
	Location = { NewLocation.x, NewLocation.y, 0.0f };
	Rotation = { 0.0f, 0.0f, NewRotation };
	Scale    = { Scale.x, Scale.y, 1.0f };

	RecalculateTransformMatrix();
}

void FTransform::SetAll3D(const glm::vec3& NewLocation, const glm::vec3& NewRotation, const glm::vec3& NewScale)
{
	Location = NewLocation;
	Rotation = NewRotation;
	Scale    = NewScale;

	RecalculateTransformMatrix();
}

void FTransform::SetLocationAndRotation2D(const glm::vec2& NewLocation, float NewRotation)
{
	Location = { NewLocation.x, NewLocation.y, 0.0f };
	Rotation = { 0.0f, 0.0f, NewRotation };

	RecalculateTransformMatrix();
}

void FTransform::SetLocationAndScale2D(const glm::vec2& NewLocation, const glm::vec2& NewScale)
{
	Location = { NewLocation.x, NewLocation.y, 0.0f };
	Scale    = { NewScale.x, NewScale.y, 1.0f };

	RecalculateTransformMatrix();
}

void FTransform::SetRotationAndScale2D(float NewRotation, const glm::vec2& NewScale)
{
	Rotation = { 0.0f, 0.0f, NewRotation };
	Scale    = { NewScale.x, NewScale.y, 1.0f  };

	RecalculateTransformMatrix();
}

void FTransform::SetLocationAndRotation3D(const glm::vec3& NewLocation, const glm::vec3& NewRotation)
{
	Location = NewLocation;
	Rotation = NewRotation;

	RecalculateTransformMatrix();
}

void FTransform::SetLocationAndScale3D(const glm::vec3& NewLocation, const glm::vec3& NewScale)
{
	Location = NewLocation;
	Scale    = NewScale;

	RecalculateTransformMatrix();
}

void FTransform::SetRotationAndScale3D(const glm::vec3& NewRotation, const glm::vec3& NewScale)
{
	Rotation = NewRotation;
	Scale    = NewScale;

	RecalculateTransformMatrix();
}

void FTransform::SetLocation2D(const glm::vec2& NewLocation)
{
	Location = { NewLocation.x, NewLocation.y, 0.0f };
	RecalculateTransformMatrix();
}

void FTransform::SetLocation3D(const glm::vec3& NewLocation)
{
	Location = NewLocation;
	RecalculateTransformMatrix();
}

void FTransform::SetRotation2D(float NewRotation)
{
	Rotation = { 0.0f, 0.0f, NewRotation };
	RecalculateTransformMatrix();
}

void FTransform::SetRotation3D(const glm::vec3& NewRotation)
{
	Rotation = NewRotation;
	RecalculateTransformMatrix();
}

void FTransform::SetScale2D(const glm::vec2& NewScale)
{
	Scale = { NewScale.x, NewScale.y, 1.0f };
	RecalculateTransformMatrix();
}

void FTransform::SetScale3D(const glm::vec3& NewScale)
{
	Scale = NewScale;
	RecalculateTransformMatrix();
}

void FTransform::RecalculateTransformMatrix()
{
	TransformMatrix = (
		glm::translate(glm::mat4(1.0f), Location) *
		glm::toMat4(glm::quat(Rotation)) *
		glm::scale(glm::mat4(1.0f), Scale)
	);
}
