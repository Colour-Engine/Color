#pragma once

#include "Core/Archive.h"

#include "Utils/CLARFInternals/Lexer.h"

struct FCLARF
{
public:
	/* Creates a string representation of an Archive in the CLARF format. This data can be used alone to recreate that entire Archive with just it. */
	static FString Generate(const FArchive& Ar);

	/* Takes in a CLARF-formatted string data and creates an Archive representing that data. */
	static FArchive Load(const FString& Data);
private:
	static void Indent(FString& Result, int32 ScopeDepth);
	static void WriteField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore);
	static void WriteBasicField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore);
	static void WriteArrayField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore);
	static void WriteGroupField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore);

	struct FLoadContext
	{
		FArchive Ar;

		struct FScope
		{
			FArchive& Ar;
			FScope* Prev = nullptr;

			FScope(FArchive& InAr, FScope* InPrev = nullptr)
				: Ar(InAr), Prev(InPrev) { }

			~FScope()
			{
			}
		};
		FScope* Scope;

		CLARF::FLexer Lexer;
		CLARF::FToken PrevToken, Token;

		FLoadContext(const FString& Data)
			: Lexer(Data)
		{
			Scope = new FScope(Ar);
			PrevToken = Token = Lexer.Lexe();
		}
	};

	static void Lexe(FLoadContext& Context);
	static bool ValidateRootIntegrity(FLoadContext& Context);

	static bool Eat(FLoadContext& Context, CLARF::ETokenType ExpectedType);
	static FArchiveFieldValue ReadField(FLoadContext& Context, const FString& FieldName, const FString& Type, const FString& ArrayType);
	static FArchiveFieldValue ReadBasicField(FLoadContext& Context, const FString& Type);
	static FArchiveFieldValue ReadArrayField(FLoadContext& Context, const FString& ElementsType);
	static FArchiveFieldValue ReadGroupField(FLoadContext& Context, const FString& FieldName);
};
