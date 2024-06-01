#include "ColorPCH.h"
#include "Archive.h"

#include "Utils/CLARF.h"

const char* ArchiveFieldValueTypeToString(EArchiveFieldValueType Type)
{
	switch (Type)
	{
	case AFV_None:    return "None";
	case AFV_Integer: return "Integer";
	case AFV_Float:   return "Float";
	case AFV_String:  return "String";
	case AFV_Array:   return "Array";
	case AFV_Group:   return "Group";
	}

	unreachable();
	return nullptr;
}

FString FArchiveFieldValue::FGroupType::Convert(EArchiveFormat Format) const
{
	switch (Format)
	{
	case EArchiveFormat::CLARF:
		return ConvertToCLARF();
	}

	unreachable();
	return "";
}

FString FArchiveFieldValue::FGroupType::ConvertToCLARF() const
{
	return CLARF::Generate(*this);
}

FArchiveFieldValue::FArchiveFieldValue(const FArchiveFieldValue& Other)
{
	Copy(Other);
}

FArchiveFieldValue::FArchiveFieldValue(FArchiveFieldValue&& Other) noexcept
{
	Move(MoveTemp(Other));
}

FArchiveFieldValue::FArchiveFieldValue(EArchiveFieldValueType Type)
{
	switch (Type)
	{
	case AFV_None:                    break;
	case AFV_Bool:    SetBool(false); break;
	case AFV_Integer: SetInteger(0);  break;
	case AFV_Float:   SetFloat(0.0f); break;
	case AFV_String:  SetString("");  break;
	case AFV_Array:   SetArray({});   break;
	case AFV_Group:   SetGroup({});   break;
	}
}

FArchiveFieldValue::FArchiveFieldValue(const FBoolType& bBoolValue)
{
	SetBool(bBoolValue);
}

FArchiveFieldValue::FArchiveFieldValue(const FIntegerType& IntegerValue)
{
	SetInteger(IntegerValue);
}

FArchiveFieldValue::FArchiveFieldValue(const FFloatType& FloatValue)
{
	SetFloat(FloatValue);
}

FArchiveFieldValue::FArchiveFieldValue(const FStringType& StringValue)
{
	SetString(StringValue);
}

FArchiveFieldValue::FArchiveFieldValue(const FArrayType& ArrayValue)
{
	SetArray(ArrayValue);
}

FArchiveFieldValue::FArchiveFieldValue(const FGroupType& GroupValue)
{
	SetGroup(GroupValue);
}

FArchiveFieldValue::~FArchiveFieldValue()
{
	Release();
}

FArchiveFieldValue& FArchiveFieldValue::operator=(const FArchiveFieldValue& Other)
{
	Copy(Other);
	return *this;
}

FArchiveFieldValue& FArchiveFieldValue::operator=(FArchiveFieldValue&& Other) noexcept
{
	Move(MoveTemp(Other));
	return *this;
}

FArchiveFieldValue& FArchiveFieldValue::operator=(const FBoolType& bBoolValue)
{
	SetBool(bBoolValue);
	return *this;
}

FArchiveFieldValue& FArchiveFieldValue::operator=(const FIntegerType& IntegerValue)
{
	SetInteger(IntegerValue);
	return *this;
}

FArchiveFieldValue& FArchiveFieldValue::operator=(const FFloatType& FloatValue)
{
	SetFloat(FloatValue);
	return *this;
}

FArchiveFieldValue& FArchiveFieldValue::operator=(const FStringType& StringValue)
{
	SetString(StringValue);
	return *this;
}

FArchiveFieldValue& FArchiveFieldValue::operator=(const FArrayType& ArrayValue)
{
	SetArray(ArrayValue);
	return *this;
}

FArchiveFieldValue& FArchiveFieldValue::operator=(const FGroupType& GroupValue)
{
	SetGroup(GroupValue);
	return *this;
}

bool FArchiveFieldValue::SetBool(const FBoolType& bBoolValue)
{
	Release();
	Value = new FBoolType(bBoolValue);
	Type = AFV_Bool;
	return true;
}

bool FArchiveFieldValue::SetInteger(const FIntegerType& IntegerValue)
{
	Release();
	Value = new FIntegerType(IntegerValue);
	Type = AFV_Integer;
	return true;
}

bool FArchiveFieldValue::SetFloat(const FFloatType& FloatValue)
{
	Release();
	Value = new FFloatType(FloatValue);
	Type = AFV_Float;
	return true;
}

bool FArchiveFieldValue::SetString(const FStringType& StringValue)
{
	Release();
	Value = new FStringType(StringValue);
	Type = AFV_String;
	return true;
}

bool FArchiveFieldValue::SetArray(const FArrayType& ArrayValue)
{
	EArchiveFieldValueType ArrayType = ArrayValue.Num() > 0 ? ArrayValue[0].Type : AFV_None;
	verifyf(ArrayType != AFV_Array && ArrayType != AFV_Group, "Arrays may not contain array or group values!");

	if (ArrayType != AFV_None)
	{
		for (const FArchiveFieldValue& AVF : ArrayValue)
		{
			if (AVF.Type != ArrayType)
			{
				CL_CORE_ERROR("Array type inconsistency!");
				return false;
			}
		}
	}

	Release();
	Value = new FArrayType(ArrayValue);
	Type = AFV_Array;
	return true;
}

bool FArchiveFieldValue::SetGroup(const FGroupType& GroupValue)
{
	Release();
	Value = new FGroupType(GroupValue);
	Type = AFV_Group;
	return true;
}

void FArchiveFieldValue::Release()
{
	delete Value;
	Value = nullptr;
	Type = AFV_None;
}

void FArchiveFieldValue::Copy(const FArchiveFieldValue& Other)
{
	Release();

	switch (Other.Type)
	{
	case AFV_None: break;
	case AFV_Bool:
	{
		Value = new FBoolType(Other.AsBool());
		break;
	}
	case AFV_Integer:
	{
		Value = new FIntegerType(Other.AsInteger());
		break;
	}
	case AFV_Float:
	{
		Value = new FFloatType(Other.AsFloat());
		break;
	}
	case AFV_String:
	{
		Value = new FString(Other.AsString());
		break;
	}
	case AFV_Array:
	{
		Value = new FArrayType(Other.AsArray());
		break;
	}
	case AFV_Group:
	{
		Value = new FGroupType(Other.AsGroup());
		break;
	}
	}

	Type = Other.Type;
}

void FArchiveFieldValue::Move(FArchiveFieldValue&& Other)
{
	Release();

	Value = Other.Value;
	Type = Other.Type;

	Other.Value = nullptr;
	Other.Type = AFV_None;
}
