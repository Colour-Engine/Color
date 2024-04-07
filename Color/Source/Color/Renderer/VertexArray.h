#pragma once

#include "Renderer/Buffer.h"

class FVertexArray
{
public:
	virtual ~FVertexArray() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void AddVertexBuffer(const TRef<FVertexBuffer>& VertexBuffer) = 0;
	virtual void SetIndexBuffer(const TRef<FIndexBuffer>& IndexBuffer) = 0;

	virtual const TArray<TRef<FVertexBuffer>>& GetVertexBuffers() const = 0;
	virtual const TRef<FIndexBuffer>& GetIndexBuffer() const = 0;

	static TRef<FVertexArray> New();
};
