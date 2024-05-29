#pragma once

#include "Core/Layer.h"
#include "Scene/Scene.h"

class FSandbox2D : public FLayer
{
public:
	virtual void OnAttach() override;
	virtual void OnTick(float DeltaTime) override;
	virtual void OnDetach() override;
private:
	TRef<FScene> Scene;
};
