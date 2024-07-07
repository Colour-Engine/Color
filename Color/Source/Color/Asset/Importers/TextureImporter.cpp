#include "ColorPCH.h"
#include "TextureImporter.h"

#include "Project/Project.h"
#include "Utils/Timer.h"

#include <stb/stb_image.h>

TRef<FTexture2D> FTextureImporter::ImportTexture2D(HAsset Asset, const FAssetMetadata& Metadata)
{
	return LoadTexture2D(FProject::GetActive()->GetAssetPath(Metadata.Filepath));
}

TRef<FTexture2D> FTextureImporter::LoadTexture2D(const FString& Filepath)
{
	int Width, Height, NumChannels;
	stbi_set_flip_vertically_on_load(1);

	FBuffer Data;
	float TimeLoad = 0.0f;
	{
		FTimer Timer;
		Data.Data = stbi_load(*Filepath, &Width, &Height, &NumChannels, 0);
		TimeLoad = Timer.ElapsedMs();
	}

	if (!Data.Data)
	{
		CL_CORE_ERROR("Failed to load texture from file '%s'. stbi_load returned NULL.", *Filepath);
		return nullptr;
	}

	CL_CORE_WARN("Loading texture from file '%s' took '%f' ms.", *Filepath, TimeLoad);
	Data.Size = Width * Height * NumChannels;

	FTextureSpecification TexSpecification;
	TexSpecification.Width = Width;
	TexSpecification.Height = Height;

	switch (NumChannels)
	{
	case 3:
	{
		TexSpecification.ImageFormat = EImageFormat::RGB8;
		break;
	}
	case 4:
	{
		TexSpecification.ImageFormat = EImageFormat::RGBA8;
		break;
	}
	default:
	{
		CL_CORE_ERROR("Couldn't load texture from file '%s' because it has an invalid number of channels (%d). Color only supports 3 and 4.", *Filepath, NumChannels);
		Data.FreeAndRelease();
		return nullptr;
	}
	}

	TRef<FTexture2D> Texture = FTexture2D::New(TexSpecification, Data);
	Texture->SetPath(Filepath);
	Data.FreeAndRelease();

	return Texture;
}
