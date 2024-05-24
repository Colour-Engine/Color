#include "ColorPCH.h"
#include "ECSTypes.h"

// IDs with this Impl always start from 1 because 0 is a special value representing invalidity.
#define NextFuncImpl(Type) static Type Last = 1; return Last++;

EntityRef NextEntityRef()
{
	NextFuncImpl(EntityRef);
}

ComponentID NextComponentTypeID()
{
	NextFuncImpl(ComponentID);
}
