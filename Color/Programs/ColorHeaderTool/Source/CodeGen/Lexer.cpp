#include "HeaderToolPCH.h"
#include "Lexer.h"

#include <ctype.h>

FLexer::FLexer(const FString& InFilepath, const FString& InSource, uint_t InPosition)
	: Filepath(InFilepath), Source(InSource)
{
	if (InPosition >= InSource.Len())
	{
		printf("Lexer starting position goes out ouf the bounds of the provided source string. (SrcLen: %d >= Pos: %d)\n", (uint32) InSource.Len(), (uint32) InPosition);
		exit(1);
	}

	Position = InPosition;
	Current  = InSource[Position];
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
	SkipAnyComment();

	if (isalpha(Current))
	{
		return CollectIdentifier();
	}

	if (isdigit(Current))
	{
		// Can end up with any of these types: Integer, Float, Double
		return CollectNumber();
	}

	if (Current == '.')
	{
		if (CanAdvance() && isdigit(Source[Position + 1]))
		{
			Advance(); // Skip dot
			return CollectNumber(true, "0.");
		}
		else
		{
			return AdvanceWithToken({ Current, ETokenType::Dot });
		}
	}

	switch (Current)
	{
	case '\"': return CollectString();
	case '#': return AdvanceWithToken({ Current, ETokenType::Hash });
	case '(': return AdvanceWithToken({ Current, ETokenType::LeftParantheses });
	case ')': return AdvanceWithToken({ Current, ETokenType::RightParantheses });
	case ':': return AdvanceWithToken({ Current, ETokenType::Colon });
	case '{': return AdvanceWithToken({ Current, ETokenType::LeftBracket });
	case '}': return AdvanceWithToken({ Current, ETokenType::RightBracket });
	case ';': return AdvanceWithToken({ Current, ETokenType::Semicolon });
	case ',': return AdvanceWithToken({ Current, ETokenType::Comma });
	case '<': return AdvanceWithToken({ Current, ETokenType::LeftAngle });
	case '>': return AdvanceWithToken({ Current, ETokenType::RightAngle });
	case '+': return AdvanceWithToken({ Current, ETokenType::Plus });
	case '-': return AdvanceWithToken({ Current, ETokenType::Minus });
	case '*': return AdvanceWithToken({ Current, ETokenType::Asterisk });
	case '/': return AdvanceWithToken({ Current, ETokenType::LeftSlash });
	case '%': return AdvanceWithToken({ Current, ETokenType::Percent });
	case '&': return AdvanceWithToken({ Current, ETokenType::Ampersand });
	case '=': return AdvanceWithToken({ Current, ETokenType::Equals });
	case '!': return AdvanceWithToken({ Current, ETokenType::Exclamation });
	case '\'': return CollectChar();
	case '^': return AdvanceWithToken({ Current, ETokenType::Pow });
	case '?': return AdvanceWithToken({ Current, ETokenType::Question });
	case '|': return AdvanceWithToken({ Current, ETokenType::Pipe });
	case '\0': return AdvanceWithToken({ '\0', ETokenType::EoF });
	}

	return AdvanceWithToken({ '\0', ETokenType::None });
}

void FLexer::Advance()
{
	if (CanAdvance())
	{
		Current = Source[++Position];
	}
	else
	{
		Current = '\0';
	}
}

const FToken& FLexer::AdvanceWithToken(const FToken& Token)
{
	Advance();
	return Token;
}

FToken& FLexer::AdvanceWithToken(FToken& Token)
{
	Advance();
	return Token;
}

void FLexer::SkipWhitespace()
{
	while (isspace(Current))
	{
		Advance();
	}
}

void FLexer::SkipAnyComment()
{
	if (HasComment(ECommentType::Singleline))
	{
		SkipComment(ECommentType::Singleline);
	}
	else if (HasComment(ECommentType::Mutiline))
	{
		SkipComment(ECommentType::Mutiline);
	}
}

void FLexer::SkipComment(ECommentType CommentType)
{
	switch (CommentType)
	{
	case ECommentType::Singleline:
	{
		SkipSinglelineComment();
		break;
	}
	case ECommentType::Mutiline:
	{
		SkipMultilineComment();
		break;
	}
	}
}

void FLexer::SkipSinglelineComment()
{
	// No need to do Advance two times to skip the opening slashes, the while loop below will do it hence it's conditions.
	while (Current != '\n' && CanAdvance())
	{
		Advance();
	}

	Advance(); // Skip endl character itself
	SkipWhitespace(); // If necessary.
	SkipAnyComment(); // If comments follow each other, lexing tends to get problematic so immediately search the new one after.
}

void FLexer::SkipMultilineComment()
{
	Advance(); // Skip /
	Advance(); // Skip *

SearchSlash:
	while (Current != '*' && CanAdvance())
	{
		Advance();
	}
	//asterisk found

	if (CanAdvance())
	{
		// Is the next char (/)? If so, (*/) requirement matches and the comment is over.
		if (Source[Position + 1] == '/')
		{
			Advance(); // Skip the asterisk
			Advance(); // Skip the slash

			SkipWhitespace(); // Skip whitespace if necessary
			SkipAnyComment(); // If comments follow each other, lexing tends to get problematic so immediately search the new one after.
			return;
		}
		else // Didn't match, back to searching.
		{
			Advance(); // Oop, don't forget to advance! Otherwise we'd get stuck in an infinite loop because we'd find the same asterisk over and over.
			goto SearchSlash;
		}
	}
	else
	{
		return; // eof?
	}
}

FToken FLexer::CollectIdentifier()
{
	FToken Result("", ETokenType::Identifier);

	while (isalnum(Current) || Current == '_')
	{
		Result.Value += Current;
		Advance();
	}

	return Result;
}

FToken FLexer::CollectChar()
{
	uint32 NumSkips = 0;
	auto Adv = [this, &NumSkips]() -> void
	{
		Advance();
		NumSkips++;
	};

	FToken Result("", ETokenType::Char);
	FString& Value = Result.Value;

	Adv(); // Skip opening apostrophe ('). Currently Pos=Src[CharWeWant]
	if (Current == '\\')
	{
		Value = Current; // We shall know that we had a backslash. Otherwise goofy stuff like '\0' -> '0' happen which definitely aren't the same things.
		Adv(); // skip right slash

		// Did we hit the closing apostrophe whilst expecting another char? This case is '\'.
		if (Current == '\'')
		{
			printf(
				"Closing apostrophe was hit too early. Expected another character afterward. "
				"This case was \"The '\\' Case\", there must be another character that follows the backslash (\\). [%s:%d]\n",
				*Filepath, (uint32) Position - NumSkips
			);

			exit(1);
		}
	}

	Value += Current;
	Adv(); // Try advance to skip TO the closing apostrophe.

	// Do we have the closing apostrophe?
	if (Current != '\'')
	{
		printf("The closing apostrophe couldn't be found, char literal definition isn't legal. [%s:%d]", *Filepath, (uint32) Position - NumSkips);
		exit(1);
	}

	Adv(); // Skip FROM the closing apostrophe.
	return Result;
}

FToken FLexer::CollectString()
{
	uint32 NumSkips = 0;
	auto Adv = [this, &NumSkips]() -> void
	{
		Advance();
		NumSkips++;
	};
	auto GetStart = [this, NumSkips]() -> uint_t { return Position - NumSkips; };

	FToken Result("", ETokenType::String);
	FString& Value = Result.Value;

	// Skip the opening quote.
	Adv();

	while (Current != '\"')
	{
		Value += Current;

		if (Current == '\\')
		{
			Adv();

			if (Current == '"')
			{
				printf(
					"The closing quote for the string was found too early. "
					"Expected another character that wasnt (\") after encountering a backslash (\\). [%s:%d]",
					*Filepath,
					(uint32) GetStart()
				);
			}
			
			Value += Current;
		}

		Adv();
	}

	// Skip the closing quote.
	Adv();

	return Result;
}

FToken FLexer::CollectNumber(bool bStartWithFloatingPoint, const FString& InitialValue)
{
	uint32 NumSkips = 0;
	auto Adv = [this, &NumSkips]() -> void
	{
		Advance();
		NumSkips++;
	};
	auto GetStart = [this, NumSkips]() -> uint_t { return Position - NumSkips; };

	enum class ENumberLexeMode
	{
		Undecided, // int or float or double. this is the starting point. it either never changes from int or changes to hex/bin or to double and then maybe to float.
		FloatingPoint, // float or double
		Hex, // 0x
		Bin // 0b
	} LexeMode = ENumberLexeMode::Undecided;

	TArray<char> ValidNumset;
	auto SetMode = [this, &LexeMode, &ValidNumset](ENumberLexeMode NewMode) -> void
	{
		LexeMode = NewMode;

		switch (NewMode)
		{
		case ENumberLexeMode::Undecided:
		case ENumberLexeMode::FloatingPoint:
		{
			ValidNumset = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
			break;
		}
		case ENumberLexeMode::Hex:
		{
			ValidNumset = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f' };
			break;
		}
		case ENumberLexeMode::Bin:
		{
			ValidNumset = { '0', '1' };
			break;
		}
		}
	};
	SetMode(!bStartWithFloatingPoint ? ENumberLexeMode::Undecided : ENumberLexeMode::FloatingPoint);

	FToken Result(InitialValue, !bStartWithFloatingPoint ? ETokenType::Integer : ETokenType::Double); // Start with int for now.
	FString& Value = Result.Value;

	// It never feels good having to use while true but anyway.
	while (true)
	{
		Value += Current;
		Adv();

		// C++ allows for numbers to be organized in any part of it as longs as it meets one simple condition:
		// The previous char being in the charset and the next one after the apostrophe being too.
		// Since it's a common occurrence, this handles them generically.
		if (Current == '\'')
		{
			if (ValidNumset.Contains(Source[Position - 1]) && CanAdvance() && ValidNumset.Contains(Source[Position + 1]))
			{
				Adv();
				continue;
			}
			else
			{
				printf("Invalid use of the organization apostrophe seperator. [%s:%d]", *Filepath, (uint32) GetStart());
				exit(1);
			}
		}

		switch (LexeMode)
		{
		case ENumberLexeMode::Undecided:
		{
			// M'kay...
			if (isdigit(Current))
			{
				continue;
			}

			if (Current == 'x') // Is this a cool hex value?
			{
				// Check if prev was 0 and if we only skipped one char, if so, it meets the comdition of the 0x prefix.
				if (Source[Position - 1] == '0' && NumSkips == 1)
				{
					SetMode(ENumberLexeMode::Hex);
					continue; // go on...
				}
				else
				{
					// oopsie.
					printf("Invalid specification of a hexadecimal value! [%s:%d]", *Filepath, (uint32) GetStart());
					exit(1);
				}
			}
			else if (Current == 'b') // Hmm, perhaps binary? (Specialized in C++14, Color is meant for C++20 and possibly above, so always checkç
			{
				// Check if prev was 0 and if we only skipped one char, if so, it meets the comdition of the 0b prefix.
				if (Source[Position - 1] == '0' && NumSkips == 1)
				{
					SetMode(ENumberLexeMode::Bin);
					continue; // go on...
				}
				else
				{
					// oopsie.
					printf("Invalid specification of a binary value! [%s:%d]", *Filepath, (uint32) GetStart());
					exit(1);
				}
			}
			else if (Current == '.')
			{
				SetMode(ENumberLexeMode::FloatingPoint);
				Value += Current;
				Adv(); // Skip the dot we just encountered.

				// C++ allows expressions such as 0.f (float) or 0. (double), so handle those here.
				if (isdigit(Current))
				{
					// Digit found, continue lexing as usual.
					// Oops, dont't forget, we are a double for now, we might turn into a float later but this is how it is for now.
					// Just like how we assume integer at first, we assume double for now.
					Result.Type = ETokenType::Double;
					continue;
				}

				if (Current == 'f') // Scenario like 12344.f, where no fraction is provided with explicit float type so it's assumed zero. (C++ behavior)
				{
					// Advance so we skip 'f', otherwise it'd be interpreted as an identifier which is a big no no.
					Adv();

					// We will be a good person and add zero to the fraction anyway! They both evaluate to the same thing at the end.
					Value += '0';

					// We are now a certified float!
					Result.Type = ETokenType::Float;

					// Nothing to lexe anymore.
					goto BreakSpot;
				}
				else // Scenario like 1243132., where no fraction is provided and some random stuff comes after the dot. We assume this as a double.
				{
					// Again, being a good person like with the float case.
					Value += '0';

					// We are now a certified double!
					Result.Type = ETokenType::Double;

					// Nothing to lexe anymore.
					goto BreakSpot;
				}
			}

			// Not a digit and not a dot, we finished our life as an integer.
			goto BreakSpot;
		}
		case ENumberLexeMode::FloatingPoint:
		{
			if (isdigit(Current))
			{
				// Well, let's resume, I guess...
				continue;
			}

			// Are we a certified float?
			if (Current == 'f')
			{
				// Advance so we skip 'f', otherwise it'd be interpreted as an identifier which is a big no no.
				Adv();

				// Let's become a float rather than a double.
				Result.Type = ETokenType::Float;

				// Done!
				goto BreakSpot;
			}

			// Some other stuff other than a digit or an 'f' was encountered. This makes us a double.
			// We can just return immediately since everything is in the right state.
			// Token type is already double.
			goto BreakSpot;
		}
		case ENumberLexeMode::Hex:
		case ENumberLexeMode::Bin:
		{
			if (ValidNumset.Contains(Current))
			{
				continue;
			}

			// welp...
			goto BreakSpot;
		}
		}

	BreakSpot:
		break;
	}
	
	return Result;
}

bool FLexer::CanAdvance() const
{
	return Current != '\0' && Position < Source.Len();
}

bool FLexer::HasCommentPotential() const
{
	return Current == '/' && CanAdvance();
}

bool FLexer::HasComment(ECommentType Type) const
{
	if (!HasCommentPotential())
	{
		return false;
	}
	char Next = Source[Position + 1];

	switch (Type)
	{
	case ECommentType::Singleline:
	{
		return Next == '/';
	}
	case ECommentType::Mutiline:
	{
		return Next == '*';
	}
	}

	return false;
}
