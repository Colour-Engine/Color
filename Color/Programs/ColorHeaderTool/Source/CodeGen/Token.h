#pragma once

#include "Containers/String.h"

enum class ETokenType
{
	None,
	EoF, // token indicating the end of a file
	Identifier, // id [example]
	String, // "text" [example]
	Hash, // #
	LeftParantheses, // (
	RightParantheses, // )
	Colon, // :
	LeftBracket, // {
	RightBracket, // }
	Semicolon, // ;
	Comma, // ,
	LeftAngle, // <
	RightAngle, // >
	Dot, // .
	Plus, // +
	Minus, // -
	Asterisk, // *
	LeftSlash, // /
	Percent, // %
	Ampersand, // &
	Equals, // =
	Exclamation, // !
	Char, // '
	Pow, // ^
	Question, // ?
	Pipe, // |
	Integer, // 89123 [example]
	Float, // 0.234f [example]
	Double // 0.256 [example]
};

const char* TokenTypeToString(ETokenType Type);

struct FToken
{
	FString Value;
	ETokenType Type = ETokenType::None;

	FToken() = default;
	FToken(const FToken&) = default;
	FToken& operator=(const FToken&) = default;
	
	FToken(const FString& Value, ETokenType Type)
		: Value(Value), Type(Type) { }
};

template <typename OStream>
inline OStream& operator<<(OStream& Stream, const FToken& Token)
{
	return Stream << "Token: { \"" << Token.Value << "\", " << TokenTypeToString(Token.Type) << " }";
}
