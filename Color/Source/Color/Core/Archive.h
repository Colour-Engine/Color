#pragma once

#include "Core/Base.h"

#include <unordered_map>

enum class EArchiveFieldValueType
{
	None = 0,

	// Can be either "true" or "false".
	Bool,

	// A signed 32-bit integer.
	Integer,

	// A 32-bit floating point number.
	Float,

	// Sequence of characters.
	String,

	// Sequence of elements of the same type except the "Group" and "Array" type. Those types aren't valid for arrays, only Bool, Integer, Float and String are.
	Array,

	// A group exists mostly for organization purposes.
	// It can contain subvalues of any type.
	Group
};

enum class EArchiveFormat
{
	// The special format of Color Engine. (CLAR stands for Color Archive and the standard extension is .clarchive)
	CLAR
};

const char* ArchiveFieldValueTypeToString(EArchiveFieldValueType Type);

class FArchiveFieldValue
{
public:
	class FArchiveFieldValueGroupValueType
	{
	public:
		using ContainerType = std::unordered_map<FString, FArchiveFieldValue>;
	public:
		FArchiveFieldValueGroupValueType() = default;
		FArchiveFieldValueGroupValueType(const FArchiveFieldValueGroupValueType&) = default;
		FArchiveFieldValueGroupValueType& operator=(const FArchiveFieldValueGroupValueType&) = default;

		FArchiveFieldValueGroupValueType(const FArchiveFieldValue& GroupValue)
		{
			verifyf(GroupValue.Type == EArchiveFieldValueType::Group, "Expected Group type!");
			Root = GroupValue.AsGroup().Root;
		}

		FArchiveFieldValueGroupValueType(std::initializer_list<std::pair<const FString, FArchiveFieldValue>> KVPs)
			: Root(KVPs) { }

		FArchiveFieldValueGroupValueType(ContainerType& InContainer)
			: Root(InContainer) { }

		FArchiveFieldValue& SetField(const FString& FieldName, const FArchiveFieldValue& Value)
		{
			return Root[FieldName] = Value;
		}

		bool RemoveField(const FString& FieldName)
		{
			if (!HasField(FieldName))
			{
				return false;
			}

			Root.erase(FieldName);
			return true;
		}

		// Checks if field with given name exists and the type of it is equal to the given type.
		bool HasFieldWithType(const FString& FieldName, EArchiveFieldValueType Type) const
		{
			return HasField(FieldName) && GetField(FieldName).GetType() == Type;
		}

		bool HasField(const FString& FieldName) const
		{
			return Root.contains(FieldName);
		}

		FArchiveFieldValue& GetField(const FString& FieldName)
		{
			verifyf(HasField(FieldName), "Field '%s' doesn't exist within the group!", *FieldName);
			return Root.at(FieldName);
		}

		const FArchiveFieldValue& GetField(const FString& FieldName) const
		{
			verifyf(HasField(FieldName), "Field '%s' doesn't exist within the group!", *FieldName);
			return Root.at(FieldName);
		}

		FArchiveFieldValue& operator[](const FString& FieldName)
		{
			if (!HasField(FieldName))
			{
				SetField(FieldName, FArchiveFieldValue(EArchiveFieldValueType::None));
				return Root[FieldName];
			}

			return GetField(FieldName);
		}

		const FArchiveFieldValue& operator[](const FString& FieldName) const
		{
			return GetField(FieldName);
		}

		FString Convert(EArchiveFormat Format = EArchiveFormat::CLAR) const
		{
			switch (Format)
			{
			case EArchiveFormat::CLAR:
				return ConvertToCLAR();
			}

			unreachable();
			return "";
		}

		FString ConvertToCLAR() const
		{
			return "";

		}

		auto begin() { return Root.begin(); }
		auto end() { return Root.end(); }
		const auto begin() const { return Root.begin(); }
		const auto end() const { return Root.end(); }

		const auto cbegin() const { return Root.cbegin(); }
		const auto cend() const { return Root.cend(); }
	private:
		ContainerType Root;
	};
public:
	using FBoolType = bool;
	using FIntegerType = int32;
	using FFloatType = float;
	using FStringType = FString;
	using FArrayType = TArray<FArchiveFieldValue>;
	using FGroupType = FArchiveFieldValueGroupValueType;
public:
	FArchiveFieldValue() = default;
	FArchiveFieldValue(const FArchiveFieldValue& Other);
	FArchiveFieldValue(FArchiveFieldValue&& Other) noexcept;
	FArchiveFieldValue(EArchiveFieldValueType Type);
	FArchiveFieldValue(const FBoolType& bBoolValue);
	FArchiveFieldValue(const FIntegerType& IntegerValue);
	FArchiveFieldValue(const FFloatType& FloatValue);
	FArchiveFieldValue(const FStringType& StringValue);
	FArchiveFieldValue(const FArrayType& ArrayValue);
	FArchiveFieldValue(const FGroupType& GroupValue);
	~FArchiveFieldValue();

	FArchiveFieldValue& operator=(const FArchiveFieldValue& Other);
	FArchiveFieldValue& operator=(FArchiveFieldValue&& Other) noexcept;

	FArchiveFieldValue& operator=(const FBoolType& bBoolValue);
	FArchiveFieldValue& operator=(const FIntegerType& IntegerValue);
	FArchiveFieldValue& operator=(const FFloatType& FloatValue);
	FArchiveFieldValue& operator=(const FStringType& StringValue);
	FArchiveFieldValue& operator=(const FArrayType& ArrayValue);
	FArchiveFieldValue& operator=(const FGroupType& GroupValue);

	bool SetBool(const FBoolType& bBoolValue);
	bool SetInteger(const FIntegerType& IntegerValue);
	bool SetFloat(const FFloatType& FloatValue);
	bool SetString(const FStringType& StringValue);
	bool SetArray(const FArrayType& ArrayValue);
	bool SetGroup(const FGroupType& GroupValue);

	template <typename T>
	const T& As() const
	{
		check(false);
	}

	template <typename T>
	T& As()
	{
		check(false);
	}

#define DECLARE_AS_FN(DataType, AFVType) \
	template <> \
	const DataType& As<DataType>() const \
	{ \
		return Unsafe_As<DataType>(EArchiveFieldValueType::##AFVType); \
	} \
	template <> \
	DataType& As<DataType>() \
	{ \
		return Unsafe_As<DataType>(EArchiveFieldValueType::##AFVType); \
	} \
	const DataType& As##AFVType() const { return As<DataType>(); } \
	DataType& As##AFVType() { return As<DataType>(); }

	DECLARE_AS_FN(FBoolType, Bool);
	DECLARE_AS_FN(FIntegerType, Integer);
	DECLARE_AS_FN(FFloatType, Float);
	DECLARE_AS_FN(FStringType, String);
	DECLARE_AS_FN(FArrayType, Array);
	DECLARE_AS_FN(FGroupType, Group);
#undef DECLARE_AS_FN

	// Only usable when the Type == Group!
	const FArchiveFieldValue& operator[](const FString& FieldName) const
	{
		return AsGroup()[FieldName];
	}

	// Only usable when the Type == Group!
	FArchiveFieldValue& operator[](const FString& FieldName)
	{
		return AsGroup()[FieldName];
	}

	operator const FGroupType&() const { return AsGroup(); }
	operator FGroupType&() { return AsGroup(); }

	EArchiveFieldValueType GetType() const { return Type; }
private:
	void Release();
	void Copy(const FArchiveFieldValue& Other);
	void Move(FArchiveFieldValue&& Other);
private:
	template <typename T>
	const T& Unsafe_As(EArchiveFieldValueType Type) const
	{
		verifyf
		(
			this->Type != EArchiveFieldValueType::None && Type != EArchiveFieldValueType::None,
			"Invalid access request to an archive data value! Neither the actual type of the value nor the requested type can be None."
		);

		verifyf
		(
			this->Type == Type,
			"Invalid interpretation of archive data value! Actual field value type is different than the requested type. Requested: %s, got: %s",
			*ArchiveFieldValueTypeToString(Type),
			*ArchiveFieldValueTypeToString(this->Type)
		);
		verifyf(Value, "Invalid access request to an archive data value! The Value hasn't been initialized and data pointer is NULL!");

		return *(T*) Value;
	}

	template <typename T>
	T& Unsafe_As(EArchiveFieldValueType Type)
	{
		verifyf
		(
			this->Type != EArchiveFieldValueType::None && Type != EArchiveFieldValueType::None,
			"Invalid access request to an archive data value! Neither the actual type of the value nor the requested type can be None."
		);

		verifyf
		(
			this->Type == Type,
			"Invalid interpretation of archive data value! Actual field value type is different than the requested type. Requested: %s, got: %s",
			*ArchiveFieldValueTypeToString(Type),
			*ArchiveFieldValueTypeToString(this->Type)
		);
		verifyf(Value, "Invalid access request to an archive data value! The Value hasn't been initialized and data pointer is NULL!");

		return *(T*) Value;
	}
private:
	EArchiveFieldValueType Type = EArchiveFieldValueType::None;
	void* Value = nullptr;
};

using FArchive = FArchiveFieldValue::FGroupType;
