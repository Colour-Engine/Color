#pragma once

#include "Core/Archive.h"
#include "Utils/ArchiveHelpers.h"
#include "Scene/ECSTypes.h"

#define Cloned __Clone_Cloned_Component
#define CLONESTART(ThisClass, BaseComponent) \
ThisClass* Cloned = new ThisClass(); \
{ \
	BaseComponent* ParentCopy = BaseComponent::Clone(); \
	Cloned->BaseComponent::operator=(*ParentCopy); \
	delete ParentCopy; \
}
#define CLONEFINISH return Cloned

class FScene;

class FComponent
{
public:
	static const char* GetIDName() { return "FComponent"; }
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
	// This tick function is called while the Scene is paused, contrary to OnTick() which doesn't gel called when the Scene is paused.
	virtual void OnPausedTick(float DeltaTime) { }
	virtual void OnDetach() { }

	void SetEnableTick(bool bEnable);
	bool IsTickEnabled() const { return bEnableTick; }

	FScene* GetOwnerScene() const { return OwnerScene; }
	EntityRef GetOwnerRef() const { return Owner; }
	const char* GetIdentificationName() const { return IDName; }

	// DON'T USE. For internal usage only.
	void __Internal_init(EntityRef OwnerRefID, FScene* OwnersScene, const char* IDName);
private:
	bool bEnableTick = true;
private:
	// Reference to the Owner Entity.
	EntityRef Owner;

	// The scene that the Owner Entity resides in.
	FScene*   OwnerScene = nullptr;

	// IDName of this component.
	const char* IDName = nullptr;
};
