#include "ColorPCH.h"
#include "TransformComponent.h"

#include "Utils/ArchiveHelpers.h"
#include <glm/gtc/matrix_transform.hpp>

FTransformComponent::FTransformComponent()
{
	RecalculateTransformMatrix();
}

FTransformComponent::FTransformComponent(const glm::vec2& NewLocation, float NewRotation, const glm::vec2& NewScale)
{
	SetAll(NewLocation, NewRotation, NewScale);
}

FTransformComponent::FTransformComponent(const glm::vec3& NewLocation, const glm::vec3& NewRotation, const glm::vec3& NewScale)
{
	SetAll(NewLocation, NewRotation, NewScale);
}

// See the definition of this constructor variant to understand why it's commented out.
// FTransformComponent::FTransformComponent(const glm::vec2& NewLocation, float NewRotation)
// {
// 	SetLocationAndRotation(NewLocation, NewRotation);
// }

FTransformComponent::FTransformComponent(const glm::vec2& NewLocation, const glm::vec2& NewScale)
{
	SetLocationAndScale(NewLocation, NewScale);
}

FTransformComponent::FTransformComponent(float NewRotation, const glm::vec2& NewScale)
{
	SetRotationAndScale(NewRotation, NewScale);
}

FComponent* FTransformComponent::Clone() const
{
	CLONESTART(FTransformComponent);

	Cloned->Location = Location;
	Cloned->Rotation = Rotation;
	Cloned->Scale    = Scale;
	Cloned->TransformMatrix = TransformMatrix;

	CLONEFINISH;
}

void FTransformComponent::SetAll2D(const glm::vec2& NewLocation, float NewRotation, const glm::vec2& NewScale)
{
	Location = { NewLocation.x, NewLocation.y, 0.0f };
	Rotation = { 0.0f, 0.0f, NewRotation };
	Scale    = { Scale.x, Scale.y, 1.0f };

	RecalculateTransformMatrix();
}

void FTransformComponent::SetAll3D(const glm::vec3& NewLocation, const glm::vec3& NewRotation, const glm::vec3& NewScale)
{
	Location = NewLocation;
	Rotation = NewRotation;
	Scale    = NewScale;

	RecalculateTransformMatrix();
}

void FTransformComponent::SetLocationAndRotation2D(const glm::vec2& NewLocation, float NewRotation)
{
	Location = { NewLocation.x, NewLocation.y, 0.0f };
	Rotation = { 0.0f, 0.0f, NewRotation };

	RecalculateTransformMatrix();
}

void FTransformComponent::SetLocationAndScale2D(const glm::vec2& NewLocation, const glm::vec2& NewScale)
{
	Location = { NewLocation.x, NewLocation.y, 0.0f };
	Scale    = { NewScale.x, NewScale.y, 1.0f };

	RecalculateTransformMatrix();
}

void FTransformComponent::SetRotationAndScale2D(float NewRotation, const glm::vec2& NewScale)
{
	Rotation = { 0.0f, 0.0f, NewRotation };
	Scale    = { NewScale.x, NewScale.y, 1.0f  };

	RecalculateTransformMatrix();
}

void FTransformComponent::SetLocationAndRotation3D(const glm::vec3& NewLocation, const glm::vec3& NewRotation)
{
	Location = NewLocation;
	Rotation = NewRotation;

	RecalculateTransformMatrix();
}

void FTransformComponent::SetLocationAndScale3D(const glm::vec3& NewLocation, const glm::vec3& NewScale)
{
	Location = NewLocation;
	Scale    = NewScale;

	RecalculateTransformMatrix();
}

void FTransformComponent::SetRotationAndScale3D(const glm::vec3& NewRotation, const glm::vec3& NewScale)
{
	Rotation = NewRotation;
	Scale    = NewScale;

	RecalculateTransformMatrix();
}

void FTransformComponent::SetLocation2D(const glm::vec2& NewLocation)
{
	Location = { NewLocation.x, NewLocation.y, 0.0f };
	RecalculateTransformMatrix();
}

void FTransformComponent::SetLocation3D(const glm::vec3& NewLocation)
{
	Location = NewLocation;
	RecalculateTransformMatrix();
}

void FTransformComponent::SetRotation2D(float NewRotation)
{
	Rotation = { 0.0f, 0.0f, NewRotation };
	RecalculateTransformMatrix();
}

void FTransformComponent::SetRotation3D(const glm::vec3& NewRotation)
{
	Rotation = NewRotation;
	RecalculateTransformMatrix();
}

void FTransformComponent::SetScale2D(const glm::vec2& NewScale)
{
	Scale = { NewScale.x, NewScale.y, 1.0f };
	RecalculateTransformMatrix();
}

void FTransformComponent::SetScale3D(const glm::vec3& NewScale)
{
	Scale = NewScale;
	RecalculateTransformMatrix();
}

void FTransformComponent::RecalculateTransformMatrix()
{
	TransformMatrix = (
		glm::translate(glm::mat4(1.0f), Location) *
		glm::toMat4(glm::quat(Rotation)) *
		glm::scale(glm::mat4(1.0f), Scale)
	);
}
