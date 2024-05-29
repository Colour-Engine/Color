#include "ColorPCH.h"
#include "Component.h"

FComponent* FComponent::Clone() const
{
	FComponent* Copy = new FComponent();
	Copy->__Internal_init(Owner, OwnerScene, IDName);

	return Copy;
}

void FComponent::SetEnableTick(bool bEnable)
{
	bEnableTick = bEnable;
}

void FComponent::__Internal_init(EntityRef OwnerRefID, FScene* OwnersScene, const char* IDName)
{
	this->Owner = OwnerRefID;
	this->OwnerScene = OwnersScene;
	this->IDName = IDName;
}
