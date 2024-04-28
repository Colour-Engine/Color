#include "HeaderToolPCH.h"
#include "Token.h"

const char* TokenTypeToString(ETokenType Type)
{
	switch (Type)
	{
	case ETokenType::None: return "None/Unknown";
	case ETokenType::EoF: return "EoF";
	case ETokenType::Identifier: return "Identifier";
	case ETokenType::String: return "String";
	case ETokenType::Hash: return "Hash";
	case ETokenType::LeftParantheses: return "LeftParantheses";
	case ETokenType::RightParantheses: return "RightParantheses";
	case ETokenType::Colon: return "Colon";
	case ETokenType::LeftBracket: return "LeftBracket";
	case ETokenType::RightBracket: return "RightBracket";
	case ETokenType::Semicolon: return "Semicolon";
	case ETokenType::Comma: return "Comma";
	case ETokenType::LeftAngle: return "LeftAngle";
	case ETokenType::RightAngle: return "RightAngle";
	case ETokenType::Dot: return "Dot";
	case ETokenType::Plus: return "Plus";
	case ETokenType::Minus: return "Minus";
	case ETokenType::Asterisk: return "Asterisk";
	case ETokenType::LeftSlash: return "LeftSlash";
	case ETokenType::Percent: return "Percent";
	case ETokenType::Ampersand: return "Ampersand";
	case ETokenType::Equals: return "Equals";
	case ETokenType::Exclamation: return "Exclamation";
	case ETokenType::Char: return "Char";
	case ETokenType::Pow: return "Pow";
	case ETokenType::Question: return "Question";
	case ETokenType::Pipe: return "Pipe";
	case ETokenType::Integer: return "Integer";
	case ETokenType::Float: return "Float";
	case ETokenType::Double: return "Double";
	}

	return nullptr;
}
