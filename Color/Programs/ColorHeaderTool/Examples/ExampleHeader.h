#pragma once

#include "Core/Base.h"
#include "CObject/ObjectMacros.h"
#include "World/Actor.h"

// Hello! This comment right here is only here to ruin my life, by forcing me to add support for it!
/*
 * Oh that, that was nothing, nothing! Multiline comments are simply another trouble.
 */ /* Wish me luck!!!
 *
 * ** Yes I'm making this as complicated as possible to make sure it works, thanks for asking.
 */

CCLASS()
class AExampleActor : public AActor
{
	GENERATED_BODY()
public:
	AExampleActor();
public:
	CFUNCTION(ExampleMetadata1, /* WOAH! What if I appear out of nowhere right here? Work plz :) */ ExampleMetadata2)
		int SomeFunction(int Param0, const float& ABC) const;

	CPROPERTY(NoSerialize)
		int NonserializedCPropertyVariable;
public:
	TArray<int> ArrayInstance;
	char Cool = 'A';
	char Cool2 = '\0';

	int IntVar = 1000;
	int IntVar2 = 0b010111101;
	int IntVar3 = 0x12312DEADC0DEFFF;
	int IntVar4 = 0x12312'DEADC0DEFFF;

	float FltVar = 0.12412f;
	float FlVar2 = 0.f;
	float FlVar3 = 0.51f;

	double DblVar = 0.12312;
	double DblVar2 = 0.;
};
