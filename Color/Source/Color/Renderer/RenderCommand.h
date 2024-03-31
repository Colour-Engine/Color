#pragma once

#include "Renderer/RendererAPI.h"

class FRenderCommand
{
public:
	static void Init()
	{
		API->Init();
	}

	static void SetViewport(uint32 X, uint32 Y, uint32 Width, uint32 Height)
	{
		API->SetViewport(X, Y, Width, Height);
	}

	static void SetClearColor(const glm::vec4& Color)
	{
		API->SetClearColor(Color);
	}

	static void SetLineWidth(float Width)
	{
		API->SetLineWidth(Width);
	}

	static void Clear()
	{
		API->Clear();
	}
private:
	static TScope<FRendererAPI> API;
};
