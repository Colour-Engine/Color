#pragma once

#include "Misc/IntTypes.h"

using EntityRef = uint64;
using ComponentID = uint64;

EntityRef NextEntityRef();
ComponentID NextComponentTypeID();

template <typename T>
ComponentID GetComponentTypeID()
{
	static ComponentID TypeRef = NextComponentTypeID();
	return TypeRef;
}
