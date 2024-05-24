#pragma once

// Dynamic non-const cast.
template <typename To, typename From>
To* Cast(From* Object)
{
	return dynamic_cast<To*>(Object);
}

// Dynamic const cast.
template <typename To, typename From>
const To* Cast(const From* Object)
{
	return dynamic_cast<const To*>(Object);
}
