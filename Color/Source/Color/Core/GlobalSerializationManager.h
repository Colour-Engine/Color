#pragma once

#include "Scene/Scene.h"

class FGlobalSerializationManager
{
public:
	virtual FArchive SerializeScene(FScene* Scene);
	virtual bool DeserializeScene(FScene* Scene, const FArchive& Ar);

	virtual FArchive SerializeComponent(FComponent* Component, bool& bOutSuccess);
	virtual bool DeserializeComponent(const char* IDName, const FArchive& Ar, FEntity Entity);
protected:
	// For derived classes to inherit. Called whenever the parent serialization manager doesn't recognize the component it was supplied with.
	virtual FArchive SerializeUnrecognizedComponent(FComponent* Component, bool& bOutSuccess);

	// For derived classes to inherit. Called whenever the parent serialization manager doesn't recognize the component it was supplied with.
	virtual bool DeserializeUnrecognizedComponent(const char* IDName, const FArchive& Ar, FEntity Entity);
};
