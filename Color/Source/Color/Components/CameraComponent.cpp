#include "ColorPCH.h"
#include "CameraComponent.h"

FCameraComponent::FCameraComponent(bool bPrimary, bool bFixedAspectRatio)
	: bPrimary(bPrimary), bFixedAspectRatio(bFixedAspectRatio)
{
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
