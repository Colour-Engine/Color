#pragma once

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

// Structure representing the transform of an entity within a 3D world space.
class FTransform
{
public:
	FTransform();
	FTransform(const FTransform&) = default;
	FTransform& operator=(const FTransform&) = default;

	// All set 2D/3D constructors (3D has defaults since it can't have specialized constructors due to conflicting overloads)
	FTransform(const glm::vec2& NewLocation, float NewRotation = 0.0f, const glm::vec2& NewScale = glm::vec2(1.0f));
	FTransform(const glm::vec3& NewLocation, const glm::vec3& NewRotation = glm::vec3(0.0f), const glm::vec3& NewScale = glm::vec3(1.0f));

	// 2D Constructors
	// FTransform(const glm::vec2& NewLocation, float NewRotation); /* FTransform-spec-ctor0 variant essentialy takes care of this... */
	FTransform(const glm::vec2& NewLocation, const glm::vec2& NewScale);
	FTransform(float NewRotation, const glm::vec2& NewScale);

	void SetAll2D(const glm::vec2& NewLocation, float NewRotation, const glm::vec2& NewScale);
	void SetAll3D(const glm::vec3& NewLocation, const glm::vec3& NewRotation, const glm::vec3& NewScale);

	void SetAll(const glm::vec2& NewLocation, float NewRotation, const glm::vec2& NewScale)
	{
		SetAll2D(NewLocation, NewRotation, NewScale);
	}

	void SetAll(const glm::vec3& NewLocation, const glm::vec3& NewRotation, const glm::vec3& NewScale)
	{
		SetAll3D(NewLocation, NewRotation, NewScale);
	}

	void SetLocationAndRotation2D(const glm::vec2& NewLocation, float NewRotation);
	void SetLocationAndScale2D(const glm::vec2& NewLocation, const glm::vec2& NewScale);
	void SetRotationAndScale2D(float NewRotation, const glm::vec2& NewScale);

	void SetLocationAndRotation3D(const glm::vec3& NewLocation, const glm::vec3& NewRotation);
	void SetLocationAndScale3D(const glm::vec3& NewLocation, const glm::vec3& NewScale);
	void SetRotationAndScale3D(const glm::vec3& NewRotation, const glm::vec3& NewScale);

	void SetLocationAndRotation(const glm::vec2& NewLocation, float NewRotation)
	{
		SetLocationAndRotation2D(NewLocation, NewRotation);
	}

	void SetLocationAndScale(const glm::vec2& NewLocation, const glm::vec2& NewScale)
	{
		SetLocationAndScale2D(NewLocation, NewScale);
	}

	void SetRotationAndScale(float NewRotation, const glm::vec2& NewScale)
	{
		SetRotationAndScale2D(NewRotation, NewScale);
	}

	void SetLocationAndRotation(const glm::vec3& NewLocation, const glm::vec3& NewRotation)
	{
		SetLocationAndRotation3D(NewLocation, NewRotation);
	}

	void SetLocationAndScale(const glm::vec3& NewLocation, const glm::vec3& NewScale)
	{
		SetLocationAndScale3D(NewLocation, NewScale);
	}

	void SetRotationAndScale(const glm::vec3& NewRotation, const glm::vec3& NewScale)
	{
		SetRotationAndScale3D(NewRotation, NewScale);
	}

	void SetLocation2D(const glm::vec2& NewLocation);
	void SetLocation3D(const glm::vec3& NewLocation);

	void SetRotation2D(float NewRotation);
	void SetRotation3D(const glm::vec3& NewRotation);

	void SetScale2D(const glm::vec2& NewScale);
	void SetScale3D(const glm::vec3& NewScale);

	void SetLocation(const glm::vec2& NewLocation) { SetLocation2D(NewLocation); }
	void SetLocation(const glm::vec3& NewLocation) { SetLocation3D(NewLocation); }

	void SetRotation(float NewRotation) { SetRotation2D(NewRotation); }
	void SetRotation(const glm::vec3& NewRotation) { SetRotation3D(NewRotation); }

	void SetScale(const glm::vec2& NewScale) { SetLocation2D(NewScale); }
	void SetScale(const glm::vec3& NewScale) { SetLocation3D(NewScale); }

	const glm::vec2& GetLocation2D() const { return Location; }
	float GetRotation2D() const { return Location.z; }
	const glm::vec3& GetScale2D() const { return Scale; }

	const glm::vec3& GetLocation3D() const { return Location; }
	const glm::vec3& GetRotation3D() const { return Rotation; }
	const glm::vec3& GetScale3D() const { return Scale; }

	const glm::vec3& GetLocation() const { return Location; }
	const glm::vec3& GetRotation() const { return Rotation; }
	const glm::vec3& GetScale() const { return Scale; }

	const glm::mat4& GetTransformMatrix() const { return TransformMatrix; }
	glm::quat GetRotationQuat() const { return glm::toQuat(TransformMatrix); }
private:
	void RecalculateTransformMatrix();
private:
	glm::vec3 Location = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale    = { 1.0f, 1.0f, 1.0f };
	glm::mat4 TransformMatrix = glm::mat4(1.0f);
};
