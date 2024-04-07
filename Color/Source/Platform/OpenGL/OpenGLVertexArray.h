#pragma once

#include "Renderer/VertexArray.h"

class FOpenGLVertexArray : public FVertexArray
{
public:
	FOpenGLVertexArray();
	virtual ~FOpenGLVertexArray() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void AddVertexBuffer(const TRef<FVertexBuffer>& VertexBuffer) override;
	virtual void SetIndexBuffer(const TRef<FIndexBuffer>& IndexBuffer) override;

	virtual const TArray<TRef<FVertexBuffer>>& GetVertexBuffers() const override { return VertexBuffers; }
	virtual const TRef<FIndexBuffer>& GetIndexBuffer() const override { return IndexBuffer; }
private:
	uint32 RendererID = 0;
	uint32 VertexBufferIndex = 0;

	TArray<TRef<FVertexBuffer>> VertexBuffers;
	TRef<FIndexBuffer> IndexBuffer;
};
