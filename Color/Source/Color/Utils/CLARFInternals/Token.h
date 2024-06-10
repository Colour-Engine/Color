#pragma once

#include "Core/Base.h"

namespace CLARF
{
	enum class ETokenType
	{
		None,
		Identifier,
		String,
		LeftAngleBracket,
		RightAngleBracket,
		LeftCurlyBracket,
		RightCurlyBracket,
		LeftSquareBracket,
		RightSquareBracket,
		Colon,
		Comma,
		Integer,
		Float,
		EoS // End of Source
	};

	const char* TokenTypeToString(ETokenType Type);

	struct FToken final
	{
		FString Value;
		ETokenType Type = ETokenType::None;

		FString Format() const
		{
			return FString::Format("{'%s',%s}", *Value, TokenTypeToString(Type));
		}
	};
}
