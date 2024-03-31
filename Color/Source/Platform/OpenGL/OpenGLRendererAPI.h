#pragma once

#include "Renderer/RendererAPI.h"

class FOpenGLRendererAPI : public FRendererAPI
{
public:
	virtual void Init() override;
	virtual void SetViewport(uint32 X, uint32 Y, uint32 Width, uint32 Height) override;
	virtual void SetClearColor(const glm::vec4& Color) override;
	virtual void SetLineWidth(float Width) override;
	virtual void Clear() override;
};
