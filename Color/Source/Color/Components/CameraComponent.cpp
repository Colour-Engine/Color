#include "ColorPCH.h"
#include "CameraComponent.h"

FCameraComponent::FCameraComponent(bool bPrimary, bool bFixedAspectRatio)
	: bPrimary(bPrimary), bFixedAspectRatio(bFixedAspectRatio)
{
}

FArchive FCameraComponent::Serialize() const
{
	SERIALIZESTART();

	SerializeAr.SetField("Camera", Camera.Serialize());
	SerializeAr.SetField("bPrimary", bPrimary);
	SerializeAr.SetField("bFixedAspectRatio", bFixedAspectRatio);

	SERIALIZEFINISH;
}

bool FCameraComponent::Deserialize(const FArchive& Archive)
{
	DESERIALIZESTART();

	if (Archive.HasFieldWithType("Camera", EArchiveFieldValueType::Group))
	{
		Camera.Deserialize(Archive.GetField("Camera").AsGroup());
	}
	else
	{
		bDeserializeResult = false;
	}

	GetFieldChecked("bPrimary", Bool, bPrimary);
	GetFieldChecked("bFixedAspectRatio", Bool, bFixedAspectRatio);

	DESERIALIZEFINISH;
}

FComponent* FCameraComponent::Clone() const
{
	CLONESTART(FCameraComponent, FComponent);

	Cloned->Camera = Camera;
	Cloned->bPrimary = bPrimary;
	Cloned->bFixedAspectRatio = bFixedAspectRatio;

	CLONEFINISH;
}

void FCameraComponent::SetIsPrimary(bool bPrimary)
{
	this->bPrimary = bPrimary;
}

void FCameraComponent::SetUseFixedAspectRatio(bool bFixedAspectRatio)
{
	this->bFixedAspectRatio = bFixedAspectRatio;
}
