#include "ColorPCH.h"
#include "Lexer.h"

namespace CLARF
{
	extern "C" static int isalnum_or_underscore(int ch)
	{
		return (int)(isalnum(ch) || ch == '_');
	}

	FLexer::FLexer(const FString& InSource, uint_t StartPosition)
	{
		checkf(InSource.IsValidIndex(StartPosition), "CLARF Lexer got an invalid start position, the index was out of bounds!");

		Source = InSource;
		Position = StartPosition;
		Current = InSource[StartPosition];
	}

	FLexer::~FLexer()
	{
	}

	FToken FLexer::Lexe()
	{
		if (isspace(Current))
		{
			SkipWhitespace();
		}

		if (isalpha(Current) || Current == '_')
		{
			return LexeIdentifier();
		}
		
		if (isdigit(Current))
		{
			return LexeNumber(false);
		}

		switch (Current)
		{
		case '\0':
		{
			return { '\0', ETokenType::EoS };
		}
		case '-':
		{
			Advance();

			if (isdigit(Current))
			{
				return LexeNumber(true);
			}

			CL_CORE_ERROR("CLARF Lexer Error: Token '-' is unrecognized with CLARF!");
			break;
		}
		case '"':
		{
			return LexeString();
		}
		case '<': return AdvanceWithToken({ Current, ETokenType::LeftAngleBracket });
		case '>': return AdvanceWithToken({ Current, ETokenType::RightAngleBracket });
		case '{': return AdvanceWithToken({ Current, ETokenType::LeftCurlyBracket });
		case '}': return AdvanceWithToken({ Current, ETokenType::RightCurlyBracket });
		case '[': return AdvanceWithToken({ Current, ETokenType::LeftSquareBracket });
		case ']': return AdvanceWithToken({ Current, ETokenType::RightSquareBracket });
		case ':': return AdvanceWithToken({ Current, ETokenType::Colon });
		case ',': return AdvanceWithToken({ Current, ETokenType::Comma });
		default: break;
		}

		return { '\0', ETokenType::None };
	}

	void FLexer::Advance()
	{
		if (CanAdvance())
		{
			Current = Source[++Position];
		}
	}

	void FLexer::SkipWhitespace()
	{
		while (isspace(Current))
		{
			Advance();
		}
	}

	const FToken& FLexer::AdvanceWithToken(const FToken& Token)
	{
		Advance();
		return Token;
	}

	FToken FLexer::LexeIdentifier()
	{
		FToken Result("", ETokenType::Identifier);

		while (isalnum_or_underscore(Current))
		{
			Result.Value += Current;
			Advance();
		}

		return Result;
	}

	FToken FLexer::LexeString()
	{
		FToken Result("", ETokenType::String);

		Advance(); // Opening quote
		while (Current != '"')
		{
			Result.Value += Current;
			Advance();
		}
		Advance(); // Closing quote

		return Result;
	}

	FToken FLexer::LexeNumber(bool bNegativeStart)
	{
		// Start off as an Integer, if we encounter a dot whilst processing an integer, we'll switch to Float.
		FToken Result("", ETokenType::Integer);

		/* Logic loop */
		while (isdigit(Current) || Current == '.')
		{
			Result.Value += Current;

			if (Current == '.' && Result.Type == ETokenType::Integer)
			{
				Result.Type = ETokenType::Float;
			}

			Advance();
		}

		if (bNegativeStart)
		{
			Result.Value.PushFront('-');
		}

		return Result;
	}

	bool FLexer::CanAdvance() const
	{
		return Current != '\0' && Position < Source.Len();
	}
}
