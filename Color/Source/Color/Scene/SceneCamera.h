#pragma once

#include "Renderer/Camera.h"

class FSceneCamera : public FCamera
{
public:
	enum class EProjectionType
	{
		Perspective,
		Orthographic
	};
public:
	FSceneCamera();
	virtual ~FSceneCamera() = default;

	virtual FArchive Serialize() const override;
	virtual bool Deserialize(const FArchive& Archive) override;

	void SetPerspective(float VerticalFOV, float NearClip, float FarClip);
	void SetOrthographic(float Size, float NearClip, float FarClip);
	void SetViewportSize(uint32 Width, uint32 Height);

	void SetPerspectiveVerticalFOV(float VerticalFOV);
	void SetPerspectiveNearClip(float NearClip);
	void SetPerspectiveFarClip(float FarClip);

	void SetOrthographicSize(float Size);
	void SetOrthographicNearClip(float NearClip);
	void SetOrthographicFarClip(float FarClip);

	float GetPerspectiveVerticalFOV() const { return PerspectiveFOV; }
	float GetPerspectiveNearClip() const { return PerspectiveNear; }
	float GetPerspectiveFarClip() const { return PerspectiveFar; }

	float GeOrthographicSize() const { return OrthographicSize; }
	float GeOrthographicNearClip() const { return OrthographicNear; }
	float GeOrthographicFarClip() const { return OrthographicFar; }

	EProjectionType GetProjectionType() const { return ProjectionType; }
private:
	void RecalculateViewProjection();
private:
	EProjectionType ProjectionType = EProjectionType::Orthographic;

	float PerspectiveFOV = glm::radians(45.0f);
	float PerspectiveNear = 0.01f, PerspectiveFar = 1000.0f;

	float OrthographicSize = 10.0f;
	float OrthographicNear = -1.0f, OrthographicFar = 1.0f;

	float AspectRatio = 0.0f;
};
