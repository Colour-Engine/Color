#pragma once

#include "Utils/CLARFInternals/Token.h"

namespace CLARF
{
	class FLexer final
	{
	public:
		FLexer(const FString& InSource, uint_t StartPosition = 0);
		~FLexer();

		// Main function of the lexer, lexes the next token and makes the proper advancements.
		FToken Lexe();

		const FString& GetSource() const { return Source; }
		uint_t GetPosition() const { return Position; }
		char GetCurrent() const { return Current; }
	private:
		void Advance();
		void SkipWhitespace();
		const FToken& AdvanceWithToken(const FToken& Token);

		FToken LexeIdentifier();
		FToken LexeString();
		FToken LexeNumber(bool bNegativeStart);

		bool CanAdvance() const;
	private:
		FString Source;
		uint_t  Position = 0;
		char    Current  = 0;
	};
}
