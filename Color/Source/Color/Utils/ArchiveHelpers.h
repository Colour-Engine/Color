#pragma once

#include "Core/Archive.h"

#include <glm/glm.hpp>

#define ARCHIVEHELPERS_VECFIELDS_X "X"
#define ARCHIVEHELPERS_VECFIELDS_Y "Y"
#define ARCHIVEHELPERS_VECFIELDS_Z "Z"
#define ARCHIVEHELPERS_VECFIELDS_W "W"

#define SerializeAr _Serialization_Archive
#define SERIALIZESTART() ::FArchive SerializeAr
#define SERIALIZEFINISH return SerializeAr

#define bDeserializeResult _Serialization_Deserialize
#define DESERIALIZESTART() SERIALIZESTART(); bool bDeserializeResult = true
#define DESERIALIZEFINISH return bDeserializeResult

#define GetFieldChecked(FieldName, AFVType, OutValue) if (Archive.HasField(FieldName)) { OutValue = Archive.GetField(FieldName).As##AFVType(); } else { bDeserializeResult = false; }
#define GetVecFieldChecked(FieldName, VecType, OutValue) if (Archive.HasFieldWithType(FieldName, ::EArchiveFieldValueType::Group)) { bDeserializeResult = ::FArchiveHelpers::Get##VecType##Field(Archive, FieldName, OutValue) == ::FArchiveHelpers::EGetResult::TotalSuccess; } else { bDeserializeResult = false; }

struct FArchiveHelpers
{
public:
	enum class EGetResult
	{
		TotalFailure,
		PartialSuccess,
		TotalSuccess
	};
public:
	static void SetVec2Field(FArchive& Archive, const FString& FieldName, const glm::vec2& Value);
	static void SetVec3Field(FArchive& Archive, const FString& FieldName, const glm::vec3& Value);
	static void SetVec4Field(FArchive& Archive, const FString& FieldName, const glm::vec4& Value);

	static EGetResult GetVec2Field(const FArchive& Archive, const FString& FieldName, glm::vec2& OutValue);
	static EGetResult GetVec3Field(const FArchive& Archive, const FString& FieldName, glm::vec3& OutValue);
	static EGetResult GetVec4Field(const FArchive& Archive, const FString& FieldName, glm::vec4& OutValue);
};
