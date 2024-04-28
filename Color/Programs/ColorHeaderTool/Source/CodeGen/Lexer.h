#pragma once

#include "CodeGen/Token.h"

enum class ECommentType
{
	Singleline,
	Mutiline
};

class FLexer
{
public:
	FLexer() = delete;
	FLexer(const FLexer&) = default; // TODO: Maybe think about setting Pos=0
	FLexer& operator=(const FLexer&) = default; // TODO: same as above
	
	FLexer(const FString& InFilepath, const FString& InSource, uint_t InPosition = 0);
	~FLexer();

	// Main function of the lexer. Tries to lexe the next token and return it. Position is advanced as necessary.
	FToken Lexe();

	const FString& GetSource() const { return Source; }
	uint_t GetPosition() const { return Position; }
	char GetCurrent() const { return Current; }
private:
	void Advance();
	FToken& AdvanceWithToken(FToken& Token);
	const FToken& AdvanceWithToken(const FToken& Token);

	void SkipWhitespace();
	void SkipAnyComment();
	void SkipComment(ECommentType CommentType);
	void SkipSinglelineComment();
	void SkipMultilineComment();

	FToken CollectIdentifier();
	FToken CollectChar();
	FToken CollectString();
	FToken CollectNumber();

	bool CanAdvance() const;
	bool HasCommentPotential() const;
	bool HasComment(ECommentType Type) const;
private:
	// Filepath is used for debug output information.
	FString Filepath = "";
	FString Source   = "";
	uint_t  Position =  0;
	char    Current  = '\0';
};
