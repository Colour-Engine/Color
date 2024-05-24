#pragma once

#include "Core/Base.h"

/**
 * @brief Layers attach to a LayerStack.
 */
class FLayer
{
public:
	FLayer(const FString& InName = "Unnamed Layer");
	virtual ~FLayer() = default;

	virtual void OnAttach() { }
	virtual void OnTick(float DeltaTime) { }
	virtual void OnDetach() { }

	const FString& GetName() const { return Name; }
private:
	FString Name;
};
