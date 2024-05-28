#include "ColorPCH.h"
#include "SceneCamera.h"

#include "Utils/ArchiveHelpers.h"

#include <glm/gtc/matrix_transform.hpp>

FSceneCamera::FSceneCamera()
{
	RecalculateViewProjection();
}

FArchive FSceneCamera::Serialize() const
{
	FArchive Ar;

	Ar.SetField("ProjectionType", (int32) ProjectionType);
	Ar.SetField("PerspectiveFOV", PerspectiveFOV);
	Ar.SetField("PerspectiveNear", PerspectiveNear);
	Ar.SetField("PerspectiveFar", PerspectiveFar);
	Ar.SetField("OrthographicSize", OrthographicSize);
	Ar.SetField("OrthographicNear", OrthographicNear);
	Ar.SetField("OrthographicFar", OrthographicFar);
	Ar.SetField("AspectRatio", AspectRatio);

	return Ar;
}

bool FSceneCamera::Deserialize(const FArchive& Archive)
{
	DESERIALIZESTART();

	GetFieldChecked("ProjectionType", Integer, (int32&) ProjectionType);
	GetFieldChecked("PerspectiveFOV", Float, PerspectiveFOV);
	GetFieldChecked("PerspectiveNear", Float, PerspectiveNear);
	GetFieldChecked("PerspectiveFar", Float, PerspectiveFar);
	GetFieldChecked("OrthographicSize", Float, OrthographicSize);
	GetFieldChecked("OrthographicNear", Float, OrthographicNear);
	GetFieldChecked("OrthographicFar", Float, OrthographicFar);
	GetFieldChecked("AspectRatio", Float, AspectRatio);

	DESERIALIZEFINISH;
}

void FSceneCamera::SetPerspective(float VerticalFOV, float NearClip, float FarClip)
{
	ProjectionType = EProjectionType::Perspective;

	PerspectiveFOV = VerticalFOV;
	PerspectiveNear = NearClip;
	PerspectiveFar = FarClip;

	RecalculateViewProjection();
}

void FSceneCamera::SetOrthographic(float Size, float NearClip, float FarClip)
{
	ProjectionType = EProjectionType::Orthographic;

	OrthographicSize = Size;
	OrthographicNear = NearClip;
	OrthographicFar = FarClip;

	RecalculateViewProjection();
}

void FSceneCamera::SetViewportSize(uint32 Width, uint32 Height)
{
	check(Width > 0 && Height > 0);
	AspectRatio = (float) Width / (float) Height; // IMPORTANT!: MAKE SURE THIS IS A FLOAT DIVISON OPERATION.
	RecalculateViewProjection();
}

void FSceneCamera::SetPerspectiveVerticalFOV(float VerticalFOV)
{
	PerspectiveFOV = VerticalFOV;

	if (ProjectionType == EProjectionType::Perspective)
	{
		RecalculateViewProjection();
	}
}

void FSceneCamera::SetPerspectiveNearClip(float NearClip)
{
	PerspectiveNear = NearClip;

	if (ProjectionType == EProjectionType::Perspective)
	{
		RecalculateViewProjection();
	}
}

void FSceneCamera::SetPerspectiveFarClip(float FarClip)
{
	PerspectiveFar = FarClip;

	if (ProjectionType == EProjectionType::Perspective)
	{
		RecalculateViewProjection();
	}
}

void FSceneCamera::SetOrthographicSize(float Size)
{
	OrthographicSize = Size;

	if (ProjectionType == EProjectionType::Orthographic)
	{
		RecalculateViewProjection();
	}
}

void FSceneCamera::SetOrthographicNearClip(float NearClip)
{
	OrthographicNear = NearClip;

	if (ProjectionType == EProjectionType::Orthographic)
	{
		RecalculateViewProjection();
	}
}

void FSceneCamera::SetOrthographicFarClip(float FarClip)
{
	OrthographicFar = FarClip;

	if (ProjectionType == EProjectionType::Orthographic)
	{
		RecalculateViewProjection();
	}
}

void FSceneCamera::RecalculateViewProjection()
{
	switch (ProjectionType)
	{
	case EProjectionType::Perspective:
	{
		Projection = glm::perspective(PerspectiveFOV, AspectRatio, PerspectiveNear, PerspectiveFar);
		break;
	}
	case EProjectionType::Orthographic:
	{
		float Left   = -OrthographicSize * AspectRatio * 0.5f;
		float Right  =  OrthographicSize * AspectRatio * 0.5f;

		float Bottom = -OrthographicSize * 0.5f;
		float Top    =  OrthographicSize * 0.5f;

		Projection = glm::ortho
		(
			Left, Right,
			Bottom, Top,
			OrthographicNear, OrthographicFar
		);

		break;
	}
	}
}
