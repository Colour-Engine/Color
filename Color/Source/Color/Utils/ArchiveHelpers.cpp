#include "ColorPCH.h"
#include "ArchiveHelpers.h"

void FArchiveHelpers::SetVec2Field(FArchive& Archive, const FString& FieldName, const glm::vec2& Value)
{
	FArchiveFieldValue AFV
	({
		{
			ARCHIVEHELPERS_VECFIELDS_X,
			Value.x
		},
		{
			ARCHIVEHELPERS_VECFIELDS_Y,
			Value.y
		}
	});

	return Archive.SetField(FieldName, MoveTemp(AFV));
}

void FArchiveHelpers::SetVec3Field(FArchive& Archive, const FString& FieldName, const glm::vec3& Value)
{
	FArchiveFieldValue AFV
	({
		{
			ARCHIVEHELPERS_VECFIELDS_X,
			Value.x
		},
		{
			ARCHIVEHELPERS_VECFIELDS_Y,
			Value.y
		},
		{
			ARCHIVEHELPERS_VECFIELDS_Z,
			Value.z
		}
	});

	return Archive.SetField(FieldName, MoveTemp(AFV));
}

void FArchiveHelpers::SetVec4Field(FArchive& Archive, const FString& FieldName, const glm::vec4& Value)
{
	FArchiveFieldValue AFV
	({
		{
			ARCHIVEHELPERS_VECFIELDS_X,
			Value.x
		},
		{
			ARCHIVEHELPERS_VECFIELDS_Y,
			Value.y
		},
		{
			ARCHIVEHELPERS_VECFIELDS_Z,
			Value.z
		},
		{
			ARCHIVEHELPERS_VECFIELDS_W,
			Value.w
		}
	});

	return Archive.SetField(FieldName, MoveTemp(AFV));
}

FArchiveHelpers::EGetResult FArchiveHelpers::GetVec2Field(const FArchive& Archive, const FString& FieldName, glm::vec2& OutValue)
{
	if (!Archive.HasField(FieldName))
	{
		return EGetResult::TotalFailure;
	}

	const FArchiveFieldValue& Value = Archive.GetField(FieldName);
	if (Value.GetType() != EArchiveFieldValueType::Group)
	{
		return EGetResult::TotalFailure;
	}

	const FArchiveFieldValue::FGroupType& Group = Value.AsGroup();
	int32 NumFailures = 0;

	if (Group.HasFieldWithType(ARCHIVEHELPERS_VECFIELDS_X, EArchiveFieldValueType::Float))
	{
		OutValue.x = Group.GetField(ARCHIVEHELPERS_VECFIELDS_X).AsFloat();
	}
	else
	{
		NumFailures++;
	}

	if (Group.HasFieldWithType(ARCHIVEHELPERS_VECFIELDS_Y, EArchiveFieldValueType::Float))
	{
		OutValue.y = Group.GetField(ARCHIVEHELPERS_VECFIELDS_Y).AsFloat();
	}
	else
	{
		NumFailures++;
	}

	return NumFailures == 0 ? EGetResult::TotalFailure : NumFailures == 2 ? EGetResult::TotalFailure : EGetResult::PartialSuccess;
}

FArchiveHelpers::EGetResult FArchiveHelpers::GetVec3Field(const FArchive& Archive, const FString& FieldName, glm::vec3& OutValue)
{
	if (!Archive.HasField(FieldName))
	{
		return EGetResult::TotalFailure;
	}

	const FArchiveFieldValue& Value = Archive.GetField(FieldName);
	if (Value.GetType() != EArchiveFieldValueType::Group)
	{
		return EGetResult::TotalFailure;
	}

	const FArchiveFieldValue::FGroupType& Group = Value.AsGroup();
	int32 NumFailures = 0;

	if (Group.HasFieldWithType(ARCHIVEHELPERS_VECFIELDS_X, EArchiveFieldValueType::Float))
	{
		OutValue.x = Group.GetField(ARCHIVEHELPERS_VECFIELDS_X).AsFloat();
	}
	else
	{
		NumFailures++;
	}

	if (Group.HasFieldWithType(ARCHIVEHELPERS_VECFIELDS_Y, EArchiveFieldValueType::Float))
	{
		OutValue.y = Group.GetField(ARCHIVEHELPERS_VECFIELDS_Y).AsFloat();
	}
	else
	{
		NumFailures++;
	}

	if (Group.HasFieldWithType(ARCHIVEHELPERS_VECFIELDS_Z, EArchiveFieldValueType::Float))
	{
		OutValue.z = Group.GetField(ARCHIVEHELPERS_VECFIELDS_Z).AsFloat();
	}
	else
	{
		NumFailures++;
	}

	return NumFailures == 0 ? EGetResult::TotalFailure : NumFailures == 3 ? EGetResult::TotalFailure : EGetResult::PartialSuccess;
}

FArchiveHelpers::EGetResult FArchiveHelpers::GetVec4Field(const FArchive& Archive, const FString& FieldName, glm::vec4& OutValue)
{
	if (!Archive.HasField(FieldName))
	{
		return EGetResult::TotalFailure;
	}

	const FArchiveFieldValue& Value = Archive.GetField(FieldName);
	if (Value.GetType() != EArchiveFieldValueType::Group)
	{
		return EGetResult::TotalFailure;
	}

	const FArchiveFieldValue::FGroupType& Group = Value.AsGroup();
	int32 NumFailures = 0;

	if (Group.HasFieldWithType(ARCHIVEHELPERS_VECFIELDS_X, EArchiveFieldValueType::Float))
	{
		OutValue.x = Group.GetField(ARCHIVEHELPERS_VECFIELDS_X).AsFloat();
	}
	else
	{
		NumFailures++;
	}

	if (Group.HasFieldWithType(ARCHIVEHELPERS_VECFIELDS_Y, EArchiveFieldValueType::Float))
	{
		OutValue.y = Group.GetField(ARCHIVEHELPERS_VECFIELDS_Y).AsFloat();
	}
	else
	{
		NumFailures++;
	}

	if (Group.HasFieldWithType(ARCHIVEHELPERS_VECFIELDS_Z, EArchiveFieldValueType::Float))
	{
		OutValue.z = Group.GetField(ARCHIVEHELPERS_VECFIELDS_Z).AsFloat();
	}
	else
	{
		NumFailures++;
	}

	if (Group.HasFieldWithType(ARCHIVEHELPERS_VECFIELDS_W, EArchiveFieldValueType::Float))
	{
		OutValue.w = Group.GetField(ARCHIVEHELPERS_VECFIELDS_W).AsFloat();
	}
	else
	{
		NumFailures++;
	}

	return NumFailures == 0 ? EGetResult::TotalFailure : NumFailures == 4 ? EGetResult::TotalFailure : EGetResult::PartialSuccess;
}
