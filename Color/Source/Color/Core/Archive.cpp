#include "ColorPCH.h"
#include "Archive.h"

const char* ArchiveFieldValueTypeToString(EArchiveFieldValueType Type)
{
	switch (Type)
	{
	case EArchiveFieldValueType::None:    return "None";
	case EArchiveFieldValueType::Integer: return "Integer";
	case EArchiveFieldValueType::Float:   return "Float";
	case EArchiveFieldValueType::String:  return "String";
	case EArchiveFieldValueType::Array:   return "Array";
	case EArchiveFieldValueType::Group:   return "Group";
	}

	unreachable();
	return nullptr;
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
	case EArchiveFieldValueType::None: break;
	case EArchiveFieldValueType::Bool: SetBool(false); break;
	case EArchiveFieldValueType::Integer: SetInteger(0); break;
	case EArchiveFieldValueType::Float: SetFloat(0.0f); break;
	case EArchiveFieldValueType::String: SetString(""); break;
	case EArchiveFieldValueType::Array: SetArray({}); break;
	case EArchiveFieldValueType::Group: SetGroup({}); break;
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
	Type = EArchiveFieldValueType::Bool;
	return true;
}

bool FArchiveFieldValue::SetInteger(const FIntegerType& IntegerValue)
{
	Release();
	Value = new FIntegerType(IntegerValue);
	Type = EArchiveFieldValueType::Integer;
	return true;
}

bool FArchiveFieldValue::SetFloat(const FFloatType& FloatValue)
{
	Release();
	Value = new FFloatType(FloatValue);
	Type = EArchiveFieldValueType::Float;
	return true;
}

bool FArchiveFieldValue::SetString(const FStringType& StringValue)
{
	Release();
	Value = new FStringType(StringValue);
	Type = EArchiveFieldValueType::String;
	return true;
}

bool FArchiveFieldValue::SetArray(const FArrayType& ArrayValue)
{
	EArchiveFieldValueType ArrayType = ArrayValue.Num() > 0 ? ArrayValue[0].Type : EArchiveFieldValueType::None;
	verifyf(ArrayType != EArchiveFieldValueType::Array && ArrayType != EArchiveFieldValueType::Group, "Arrays may not contain array or group values!");

	if (ArrayType != EArchiveFieldValueType::None)
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
	Type = EArchiveFieldValueType::Array;
	return true;
}

bool FArchiveFieldValue::SetGroup(const FGroupType& GroupValue)
{
	Release();
	Value = new FGroupType(GroupValue);
	Type = EArchiveFieldValueType::Group;
	return true;
}

void FArchiveFieldValue::Release()
{
	delete Value;
	Value = nullptr;
	Type = EArchiveFieldValueType::None;
}

void FArchiveFieldValue::Copy(const FArchiveFieldValue& Other)
{
	Release();

	switch (Other.Type)
	{
	case EArchiveFieldValueType::None: break;
	case EArchiveFieldValueType::Bool:
	{
		Value = new FBoolType(Other.AsBool());
		break;
	}
	case EArchiveFieldValueType::Integer:
	{
		Value = new FIntegerType(Other.AsInteger());
		break;
	}
	case EArchiveFieldValueType::Float:
	{
		Value = new FFloatType(Other.AsFloat());
		break;
	}
	case EArchiveFieldValueType::String:
	{
		Value = new FString(Other.AsString());
		break;
	}
	case EArchiveFieldValueType::Array:
	{
		Value = new FArrayType(Other.AsArray());
		break;
	}
	case EArchiveFieldValueType::Group:
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
	Other.Type = EArchiveFieldValueType::None;
}
