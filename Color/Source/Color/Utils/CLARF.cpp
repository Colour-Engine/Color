#include "ColorPCH.h"
#include "CLARF.h"

#define EAT(ExpectedType) if (!Eat(Context, CLARF::ETokenType::##ExpectedType)) { return {}; }

static std::unordered_map<EArchiveFieldValueType, const char*> AFVTypeIdentifiers =
{
	{ AFV_None,    "NAN" },
	{ AFV_Bool,    "BIT" },
	{ AFV_Integer, "INT" },
	{ AFV_Float,   "FLT" },
	{ AFV_String,  "STR" },
	{ AFV_Array,   "ARR" },
	{ AFV_Group,   "GRP" }
};

FString FCLARF::Generate(const FArchive& Ar)
{
	FString Result;
	int32 ScopeDepth = 0;

	WriteField(Result, "Root", Ar, ScopeDepth, false);
	return Result;
}

FArchive FCLARF::Load(const FString& Data)
{
	FLoadContext Context(Data);

	// Validate the "Root" group declbegin. Every archive has a "Root" field (type of Group) that contains all subfields.
	if (!ValidateRootIntegrity(Context))
	{
		CL_CORE_ERROR("Validation of the integrity of the \"Root\" field has failed.");
		return {};
	}

	Context.Ar = ReadGroupField(Context, "Root");
	delete Context.Scope;

	return Context.Ar;
}

void FCLARF::Lexe(FLoadContext& Context)
{
	Context.PrevToken = Context.Token;
	Context.Token = Context.Lexer.Lexe();
}

bool FCLARF::ValidateRootIntegrity(FLoadContext& Context)
{
	if (Context.Token.Value != "Root")
	{
		CL_CORE_ERROR("CLARF Loading Error: Serialized data doesn't begin with the declaration of the \"Root\" field, which is a common field all Archives in Color Engine share.");
		return false;
	}
	Lexe(Context);

	if (Context.Token.Type != CLARF::ETokenType::LeftAngleBracket)
	{
		CL_CORE_ERROR("CLARF Loading Error: Root field declaration error. Field name wasn't followed with a left angle bracket.");
		return false;
	}
	Lexe(Context);

	if (Context.Token.Value != AFVTypeIdentifiers[AFV_Group])
	{
		CL_CORE_ERROR("CLARF Loading Error: Root field declaration error. Type begin declarator (left angle bracket) wasn't followed by appropriate field value type for the \"Root\" field, which is AFV_Group (%s in data).", AFVTypeIdentifiers[AFV_Group]);
		return false;
	}
	Lexe(Context);

	if (Context.Token.Type != CLARF::ETokenType::RightAngleBracket)
	{
		CL_CORE_ERROR("CLARF Loading Error: Root field declaration error. Type declaration wasn't followed by a right angle bracket.");
		return false;
	}
	Lexe(Context);

	if (Context.Token.Type != CLARF::ETokenType::Colon)
	{
		CL_CORE_ERROR("CLARF Loading Error: Root field declaration error. Type declaration signature wasn't followed by a colon.");
		return false;
	}
	Lexe(Context);

	return true;
}

bool FCLARF::Eat(FLoadContext& Context, CLARF::ETokenType ExpectedType)
{
	if (Context.Token.Type == ExpectedType)
	{
		Lexe(Context);
		return true;
	}
	else
	{
		CL_CORE_ERROR("CLARF Loading Error. Expected token '%s', got '%s'. (Lexer position = %d)", TokenTypeToString(ExpectedType), TokenTypeToString(Context.Token.Type));
		return false;
	}
}

FArchiveFieldValue FCLARF::ReadField(FLoadContext& Context, const FString& FieldName, const FString& Type, const FString& ArrayType)
{
	if
	(
		Type == AFVTypeIdentifiers[AFV_Bool]    ||
		Type == AFVTypeIdentifiers[AFV_Integer] ||
		Type == AFVTypeIdentifiers[AFV_Float]   ||
		Type == AFVTypeIdentifiers[AFV_String]
	)
	{
		return ReadBasicField(Context, Type);
	}
	else if (Type == AFVTypeIdentifiers[AFV_Array])
	{
		return ReadArrayField(Context, ArrayType);
	}
	else if (Type == AFVTypeIdentifiers[AFV_Group])
	{
		return ReadGroupField(Context, FieldName);
	}
	else
	{
		CL_CORE_ERROR("CLARF Loading Error. Unknown field type '%s'.", *Type);
	}

	return {};
}

FArchiveFieldValue FCLARF::ReadBasicField(FLoadContext& Context, const FString& Type)
{
	FArchiveFieldValue Result;

	if (Type == AFVTypeIdentifiers[AFV_Bool])
	{
		FString Value = MoveTemp(Context.Token.Value);
		EAT(Identifier);

		Result.SetBool(Value == "true" ? true : false);
	}
	else if (Type == AFVTypeIdentifiers[AFV_Integer])
	{
		FString Value = MoveTemp(Context.Token.Value);
		EAT(Integer);

		Result.SetInteger(Value.ToInteger());
	}
	else if (Type == AFVTypeIdentifiers[AFV_Float])
	{
		FString Value = MoveTemp(Context.Token.Value);
		EAT(Float);

		Result.SetFloat(Value.ToFloat());
	}
	else if (Type == AFVTypeIdentifiers[AFV_String])
	{
		FString Value = MoveTemp(Context.Token.Value);
		EAT(String);

		Result.SetString(Value);
	}
	else
	{
		CL_CORE_FATAL("Invalid call to ReadBasicField. Given type ('%s') is not considered a basic field.", *Type);
	}

	return Result;
}

FArchiveFieldValue FCLARF::ReadArrayField(FLoadContext& Context, const FString& ElementsType)
{
	FArchiveFieldValue Result(AFV_Array);
	EAT(LeftSquareBracket);

	do
	{
		// FOR do-while initial run TYPE != COMMA so don't try to EAT.
		if (Context.Token.Type == CLARF::ETokenType::Comma)
		{
			EAT(Comma);
		}

		Result.AsArray().Add(ReadBasicField(Context, ElementsType));
	} while (Context.Token.Type == CLARF::ETokenType::Comma);

	EAT(RightSquareBracket);
	return Result;
}

FArchiveFieldValue FCLARF::ReadGroupField(FLoadContext& Context, const FString& FieldName)
{
	EAT(LeftCurlyBracket);

	FArchiveFieldValue& Result = Context.Scope->Ar.SetField(FieldName, FArchiveFieldValue(AFV_Group));
	FLoadContext::FScope* NewScope = new FLoadContext::FScope(Result, Context.Scope);
	Context.Scope = NewScope;

	while (true)
	{
		FString FieldName = MoveTemp(Context.Token.Value);
		EAT(String);

		EAT(LeftAngleBracket);
		FString Type = MoveTemp(Context.Token.Value);
		EAT(Identifier);

		FString ArrayType;
		if (Type == AFVTypeIdentifiers[AFV_Array])
		{
			EAT(LeftAngleBracket);
			ArrayType = MoveTemp(Context.Token.Value);
			EAT(Identifier);
			EAT(RightAngleBracket);
		}

		EAT(RightAngleBracket);
		EAT(Colon);

		Context.Scope->Ar.SetField(FieldName, ReadField(Context, FieldName, Type, ArrayType));

		if (Context.Token.Type == CLARF::ETokenType::Comma)
		{
			EAT(Comma);
			continue;
		}
		EAT(RightCurlyBracket);

		FLoadContext::FScope* Temp = Context.Scope->Prev;
		delete Context.Scope;
		Context.Scope = Temp;

		break;
	}

	return Result;
}

void FCLARF::Indent(FString& Result, int32 ScopeDepth)
{
	for (int32 i = 0; i < ScopeDepth; i++)
	{
		Result.Push('\t');
	}
}

void FCLARF::WriteField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore)
{
	Indent(Result, ScopeDepth);

	switch (Field.GetType())
	{
	case AFV_None:                                                                   break;
	case AFV_Bool:    WriteBasicField(Result, Name, Field, ScopeDepth, bExpectMore); break;
	case AFV_Integer: WriteBasicField(Result, Name, Field, ScopeDepth, bExpectMore); break;
	case AFV_Float:   WriteBasicField(Result, Name, Field, ScopeDepth, bExpectMore); break;
	case AFV_String:  WriteBasicField(Result, Name, Field, ScopeDepth, bExpectMore); break;
	case AFV_Array:   WriteArrayField(Result, Name, Field, ScopeDepth, bExpectMore); break;
	case AFV_Group:   WriteGroupField(Result, Name, Field, ScopeDepth, bExpectMore); break;
	}
}

void FCLARF::WriteBasicField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore)
{
	Result += FString::Format("\"%s\"<%s>: ", *Name, AFVTypeIdentifiers[Field.GetType()]);

	switch (Field.GetType())
	{
	case AFV_Bool:    Result += Field.AsBool() ? "true" : "false"; break;
	case AFV_Integer: Result += FString::FromInt(Field.AsInteger()); break;
	case AFV_Float:   Result += FString::FromFloat(Field.AsFloat()); break;
	case AFV_String:  Result += FString::Format("\"%s\"", *Field.AsString()); break;
	}

	if (bExpectMore)
	{
		Result += ",\n";
	}
	else
	{
		Result += '\n';
	}
}

void FCLARF::WriteArrayField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore)
{
	if (Field.GetArrayType() == AFV_Array || Field.GetArrayType() == AFV_Group)
	{
		CL_CORE_ERROR("FCLARF doesn't support arrays of arrays or arrays of groups!");
		return;
	}
	Result += FString::Format("\"%s\"<%s<%s>>: [ ", *Name, AFVTypeIdentifiers[AFV_Array], AFVTypeIdentifiers[Field.GetArrayType()]);

	const TArray<FArchiveFieldValue>& Array = Field.AsArray();
	int32 i = 0;

	for (const FArchiveFieldValue& Element : Array)
	{
		const bool bIsLastElement = i == Array.Num() - 1;
		if (Element.GetType() != Field.GetArrayType())
		{
			CL_CORE_ERROR("Array conversion to FCLARF failure! Element type was different than the array type!");
			continue;
		}

		switch (Element.GetType())
		{
		case AFV_Bool:    Result += Element.AsBool() ? "true" : "false"; break;
		case AFV_Integer: Result += FString::FromInt(Element.AsInteger()); break;
		case AFV_Float:   Result += FString::FromFloat(Element.AsFloat()); break;
		case AFV_String:  Result += FString::Format("\"%s\"", *Element.AsString()); break;
		default: break;
		}

		if (!bIsLastElement)
		{
			Result += ", ";
		}

		i++;
	}

	if (bExpectMore)
	{
		Result += " ],\n";
	}
	else
	{
		Result += " ]\n";
	}
}

void FCLARF::WriteGroupField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore)
{
	ScopeDepth++;
	Result += FString::Format("\"%s\"<%s>: {\n", *Name, AFVTypeIdentifiers[AFV_Group]);

	const FArchiveFieldValue::FGroupType& Group = Field.AsGroup();
	uint_t i = 0;

	for (auto&& [FieldName, FieldValue] : Group)
	{
		const bool bHasMoreElements = i != Group.Num() - 1;
		Indent(Result, ScopeDepth);

		switch (FieldValue.GetType())
		{
		case AFV_None:                                                                                  break;
		case AFV_Bool:    WriteBasicField(Result, FieldName, FieldValue, ScopeDepth, bHasMoreElements); break;
		case AFV_Integer: WriteBasicField(Result, FieldName, FieldValue, ScopeDepth, bHasMoreElements); break;
		case AFV_Float:   WriteBasicField(Result, FieldName, FieldValue, ScopeDepth, bHasMoreElements); break;
		case AFV_String:  WriteBasicField(Result, FieldName, FieldValue, ScopeDepth, bHasMoreElements); break;
		case AFV_Array:   WriteArrayField(Result, FieldName, FieldValue, ScopeDepth, bHasMoreElements); break;
		case AFV_Group:   WriteGroupField(Result, FieldName, FieldValue, ScopeDepth, bHasMoreElements); break;
		}

		i++;
	}

	ScopeDepth--;
	Indent(Result, ScopeDepth);

	if (bExpectMore)
	{
		Result += "},\n";
	}
	else
	{
		Result += "}\n";
	}
}
