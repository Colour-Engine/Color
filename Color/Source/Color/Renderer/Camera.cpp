#include "ColorPCH.h"
#include "Camera.h"

FCamera::FCamera(const glm::mat4& InProjection)
	: Projection(InProjection)
{
}

FArchive FCamera::Serialize() const
{
	FArchive Ar;
	return Ar;
}

bool FCamera::Deserialize(const FArchive& Archive)
{
	return true;
}
