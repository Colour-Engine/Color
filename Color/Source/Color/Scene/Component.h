#pragma once

#include "Scene/ECSTypes.h"

#define Cloned __Clone_Cloned_Component
#define CLONESTART(ThisType, ParentType) \
ThisType* Cloned = new ThisType(); \
{ \
	ParentType* ParentCopy = ParentType::Clone(); \
	Cloned->ParentType::operator=(*ParentCopy); \
	delete ParentCopy; \
}
#define CLONEFINISH return Cloned

class FScene;

class FComponent
{
public:
	FComponent() = default;

	// Don't use copy/move constructors with components!
	FComponent(const FComponent&) = default;

	// Don't use copy/move constructors with components!
	FComponent& operator=(const FComponent&) = default;

	virtual ~FComponent() = default;
	virtual FComponent* Clone() const;

	virtual void OnAttach(bool bReplaced) { }
	virtual void OnTick(float DeltaTime) { }
	virtual void OnDetach() { }

	void SetEnableTick(bool bEnable);
	bool IsTickEnabled() const { return bEnableTick; }

	FScene* GetOwnerScene() const { return OwnerScene; }
	EntityRef GetOwnerRef() const { return Owner; }
	ComponentID GetTypeID() const { return TypeID; }

	// DON'T USE. For internal usage only.
	void __Internal_init(EntityRef OwnerRefID, FScene* OwnersScene, ComponentID MyTypeID);
private:
	bool bEnableTick = true;
private:
	// Reference to the Owner Entity.
	EntityRef Owner;

	// The scene that the Owner Entity resides in.
	FScene*   OwnerScene = nullptr;

	// TypeID of this component.
	ComponentID TypeID;
};
