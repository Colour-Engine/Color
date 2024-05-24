#include "ColorPCH.h"
#include "Component.h"

FComponent* FComponent::Clone() const
{
	FComponent* Copy = new FComponent();
	Copy->__Internal_init(Owner, OwnerScene, TypeID);

	return Copy;
}

void FComponent::SetEnableTick(bool bEnable)
{
	bEnableTick = bEnable;
}

void FComponent::__Internal_init(EntityRef OwnerRefID, FScene* OwnersScene, ComponentID MyTypeID)
{
	Owner = OwnerRefID;
	OwnerScene = OwnersScene;
	TypeID = MyTypeID;
}
