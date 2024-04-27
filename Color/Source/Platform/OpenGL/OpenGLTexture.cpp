#include "ColorPCH.h"
#include "OpenGLTexture.h"

#include "Utils/Timer.h"

#include <stb/stb_image.h>
#include <glad/glad.h>

namespace Utils
{
	static GLenum ImageFormatToGLInternalFormat(EImageFormat Format)
	{
		switch (Format)
		{
		case EImageFormat::RGB8:  return GL_RGB8;
		case EImageFormat::RGBA8: return GL_RGBA8;
		}

		checkf(false);
		return 0x0;
	}

	static GLenum ImageFormatToGLDataFormat(EImageFormat Format)
	{
		switch (Format)
		{
		case EImageFormat::RGB8:  return GL_RGB;
		case EImageFormat::RGBA8: return GL_RGBA;
		}

		checkf(false);
		return 0x0;
	}
}

FOpenGLTexture2D::FOpenGLTexture2D(const FTextureSpecification& InSpecification)
	: Specification(InSpecification), Width(InSpecification.Width), Height(InSpecification.Height)
{
	InternalFormat = Utils::ImageFormatToGLInternalFormat(InSpecification.ImageFormat);
	DataFormat     = Utils::ImageFormatToGLDataFormat(InSpecification.ImageFormat);

	CreateOnRenderer();
}

FOpenGLTexture2D::FOpenGLTexture2D(const FString& InPath)
	: Path(InPath)
{
	int Width, Height, NumChannels;
	stbi_set_flip_vertically_on_load(1);

	stbi_uc* Data = nullptr;
	float TimeLoad = 0.0f;
	{
		FTimer Timer;
		Data = stbi_load(*InPath, &Width, &Height, &NumChannels, 0);
		TimeLoad = Timer.ElapsedMs();
	}

	if (Data)
	{
		CL_CORE_WARN("Loading texture from file '%s' took '%f' ms.", *InPath, TimeLoad);

		this->Width = Width;
		this->Height = Height;

		switch (NumChannels)
		{
		case 3:
		{
			InternalFormat = GL_RGB8;
			DataFormat = GL_RGB;

			break;
		}
		case 4:
		{
			InternalFormat = GL_RGBA8;
			DataFormat = GL_RGBA;

			break;
		}
		default:
		{
			CL_CORE_ERROR("Couldn't load texture from file '%s' because it has an invalid number of channels (%d). Color only supports 3 and 4.", *InPath, NumChannels);
			stbi_image_free(Data);
			return;
		}
		}

		if (!(InternalFormat & DataFormat))
		{
			CL_CORE_ERROR("Couldn't load texture from file '%s' because it's formatted as such that Color doesn't support it. (!(InternalFormat & DataFormat))", *InPath);
			stbi_image_free(Data);
			return;
		}

		bLoaded = true;
		CreateOnRenderer(Data);

		stbi_image_free(Data);
	}
	else
	{
		CL_CORE_ERROR("Failed to load texture from file '%s'. stbi_load returned NULL.", *InPath);
	}
}

FOpenGLTexture2D::~FOpenGLTexture2D()
{
	glDeleteTextures(1, &RendererID);
}

void FOpenGLTexture2D::CreateOnRenderer(const uint8* Data)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &RendererID);
	glTextureStorage2D(RendererID, 1, InternalFormat, Width, Height);

	glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (Data)
	{
		glTextureSubImage2D(RendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, Data);

		if (Specification.bGenerateMips)
		{
			glGenerateTextureMipmap(RendererID);
		}
	}
}

void FOpenGLTexture2D::Bind(uint32 Slot) const
{
	glBindTextureUnit(Slot, RendererID);
}

void FOpenGLTexture2D::SetData(const void* Data, uint32 Size)
{
	uint32 BPP = DataFormat == GL_RGBA ? 4 : 3; // Bytes per Pixel
	uint32 RqSize = Width * Height * BPP;

	checkf(Size == RqSize, "Data must occupy/fill/be entire texture! Expected Size: %d, Given Size: %d", RqSize, Size);
	glTextureSubImage2D(RendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, Data);
}
