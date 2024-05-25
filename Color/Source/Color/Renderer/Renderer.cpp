#include "ColorPCH.h"
#include "Renderer.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"

void FRenderer::Init()
{
	FRenderCommand::Init();
	FRenderer2D::Init();
}

void FRenderer::Shutdown()
{
	FRenderer2D::Shutdown();
}
