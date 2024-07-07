#pragma once

#include "Core/Archive.h"
#include "Utils/ArchiveHelpers.h"
#include "Scene/ECSTypes.h"

#define Cloned __Clone_Cloned_Component
#define CLONESTART(ThisClass) \
ThisClass* Cloned = new ThisClass(); \
{ \
	FComponent* ParentCopy = FComponent::Clone(); \
	Cloned->FComponent::operator=(*ParentCopy); \
	delete ParentCopy; \
}
#define CLONEFINISH return Cloned

class FScene;

// Every component inheriting from this one must provide a method as specified below:
//   static const char* GetIDName()
// That method must return a unique string for each component type.
// Also, component inheritence isn't supported. Once a class is derived from FComponent,
// never create a component that derives from that derived class. The behavior is undefined if done do.
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
	virtual void OnDetach() { }

	void SetTickRules(bool bEnable, bool bDoTickWhenPaused);
	void SetEnableTick(bool bEnable);
	void SetTickWhenPaused(bool bTick);

	bool IsTickEnabled() const { return bEnableTick; }
	bool DoesTickWhenPaused() const { return bTickWhenPaused; }

	FScene* GetOwnerScene() const { return OwnerScene; }
	EntityRef GetOwnerRef() const { return Owner; }
	const char* GetIdentificationName() const { return IDName; }

	// DON'T USE. For internal usage only.
	void __Internal_init(EntityRef OwnerRefID, FScene* OwnersScene, const char* IDName);
private:
	bool bEnableTick = true;
	bool bTickWhenPaused = false;
private:
	// Reference to the Owner Entity.
	EntityRef Owner;

	// The scene that the Owner Entity resides in.
	FScene*   OwnerScene = nullptr;

	// IDName of this component.
	const char* IDName = nullptr;
};
