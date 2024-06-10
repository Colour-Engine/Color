#include "ColorPCH.h"
#include "Token.h"

namespace CLARF
{
	const char* TokenTypeToString(ETokenType Type)
	{
		switch (Type)
		{
		case ETokenType::None:               return "None";
		case ETokenType::Identifier:         return "Identifier";
		case ETokenType::String:             return "String";
		case ETokenType::LeftAngleBracket:   return "Left Angle Bracket";
		case ETokenType::RightAngleBracket:  return "Right Angle Bracket";
		case ETokenType::LeftCurlyBracket:   return "Left Curly Bracket";
		case ETokenType::RightCurlyBracket:  return "Right Curly Bracket";
		case ETokenType::LeftSquareBracket:  return "Left Square Bracket";
		case ETokenType::RightSquareBracket: return "Right Square Bracket";
		case ETokenType::Colon:              return "Colon";
		case ETokenType::Comma:              return "Comma";
		case ETokenType::Integer:            return "Integer";
		case ETokenType::Float:              return "Float";
		case ETokenType::EoS:                return "EoS (End of Source)";
		}

		unreachable();
		return nullptr;
	}
}
