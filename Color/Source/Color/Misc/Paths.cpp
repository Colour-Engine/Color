#include "ColorPCH.h"
#include "Paths.h"

FString FPaths::GetContentDirectory()
{
	return "Content";
}

FString FPaths::GetEngineAssetsDirectory()
{
	return "EngineAssets";
}

FString FPaths::GetCacheDirectory()
{
	return "Cache";
}

FString FPaths::GetShaderCacheDirectory()
{
	return GetCacheDirectory() + "/Shader";
}

FString FPaths::GetAPIShaderCacheDirectory(const FString& API)
{
	return GetShaderCacheDirectory() + '/' + API;
}
