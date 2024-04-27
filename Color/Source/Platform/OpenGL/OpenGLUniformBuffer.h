#pragma once

#include "Renderer/UniformBuffer.h"

class FOpenGLUniformBuffer : public FUniformBuffer
{
public:
	FOpenGLUniformBuffer(uint32 Size, uint32 Binding);
	~FOpenGLUniformBuffer() override;

	virtual void SetData(const void* Data, uint32 Size, uint32 Offset = 0) override;
private:
	uint32 RendererID = 0;
};
