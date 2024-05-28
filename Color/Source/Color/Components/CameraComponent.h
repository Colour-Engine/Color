#pragma once

#include "Scene/Component.h"
#include "Scene/SceneCamera.h"

class FCameraComponent : public FComponent
{
public:
	FCameraComponent() = default;
	FCameraComponent(bool bPrimary, bool bFixedAspectRatio = false);

	virtual FArchive Serialize() const override;
	virtual bool Deserialize(const FArchive& Archive) override;
	virtual FComponent* Clone() const override;

	void SetIsPrimary(bool bPrimary);
	void SetUseFixedAspectRatio(bool bFixedAspectRatio);

	const FSceneCamera& GetCamera() const { return Camera; }
	FSceneCamera& GetCamera() { return Camera; }

	bool IsPrimary() const { return bPrimary; }
	bool HasFixedAspectRatio() const { return bFixedAspectRatio; }
private:
	FSceneCamera Camera;
	bool    bPrimary = true;
	bool    bFixedAspectRatio = false;
};
