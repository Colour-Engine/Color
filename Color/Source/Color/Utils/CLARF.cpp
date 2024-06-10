#include "ColorPCH.h"
#include "CLARF.h"

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
