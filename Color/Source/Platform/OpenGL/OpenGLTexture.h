#pragma once

#include "Renderer/Texture.h"
#include "GLType.h"

class FOpenGLTexture2D : public FTexture2D
{
public:
	FOpenGLTexture2D(const FTextureSpecification& InSpecification);
	FOpenGLTexture2D(const FString& InPath);
	~FOpenGLTexture2D() override;

	virtual void Bind(uint32 Slot = 0) const override;
	virtual void SetData(const void* Data, uint32 Size) override;

	virtual const FTextureSpecification& GetSpecification() const override { return Specification; }
	virtual const FString& GetPath() const override { return Path; }
	virtual bool IsLoaded() const override { return bLoaded; }
	virtual uint32 GetWidth() const override { return Width; }
	virtual uint32 GetHeight() const override { return Height; }
	virtual uint32 GetRendererID() const override { return RendererID; }

	virtual bool operator==(const FTexture& Other) const override
	{
		return RendererID == Other.GetRendererID();
	}
private:
	void CreateOnRenderer(const uint8* Data = nullptr);
private:
	FTextureSpecification Specification;
	FString Path;

	uint32 RendererID = 0;
	bool bLoaded = false;

	uint32 Width = 0, Height = 0;
	GLenum InternalFormat = 0x0, DataFormat = 0x0;
};
