#pragma once

#include "Core/Memory.h"
#include "Asset/Asset.h"

enum class EImageFormat
{
	None = 0,

	R8,
	RGB8,
	RGBA8,
	RGBA32F,
};

struct FTextureSpecification
{
	uint32 Width = 1;
	uint32 Height = 1;

	EImageFormat ImageFormat = EImageFormat::RGBA8;
	bool bGenerateMips = true;
};

class FTexture
{
public:
	virtual ~FTexture() = default;

	virtual void Bind(uint32 Slot = 0) const = 0;
	virtual void SetData(const void* Data, uint32 Size) = 0;

	virtual const FTextureSpecification& GetSpecification() const = 0;
	virtual const FString& GetPath() const = 0;
	virtual bool IsLoaded() const = 0;
	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;
	virtual uint32 GetRendererID() const = 0;

	virtual bool operator==(const FTexture& Other) const = 0;
};

class FTexture2D : public FTexture, public FAsset
{
public:
	IMPLEMENT_ASSET(Texture2D);
public:
	static TRef<FTexture2D> New(const FTextureSpecification& InSpecification, FBuffer Data = {});
private:
	virtual void SetPath(const FString& NewPath) = 0;
	friend class FTextureImporter;
};
