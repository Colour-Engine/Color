#pragma once

#include "Core/Archive.h"

struct FCLARF
{
	static FString Generate(const FArchive& Ar);
private:
	static void Indent(FString& Result, int32 ScopeDepth);
	static void WriteField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore);
	static void WriteBasicField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore);
	static void WriteArrayField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore);
	static void WriteGroupField(FString& Result, const FString& Name, const FArchiveFieldValue& Field, int32& ScopeDepth, bool bExpectMore);
};
