#pragma once

#include "Renderer/Buffer.h"
#include "GLType.h"

struct FOpenGLBufferUtils
{
public:
	static void ConstructBuffer(uint32& RendererID, GLenum Target, const void* InData, uint32 InSize, GLenum Usage);
};

class FOpenGLVertexBuffer : public FVertexBuffer
{
public:
	FOpenGLVertexBuffer(uint32 InSize);
	FOpenGLVertexBuffer(const float* InVertices, uint32 InSize);
	virtual ~FOpenGLVertexBuffer() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void SetData(const void* InData, uint32 InSize) override;
	virtual void SetLayout(const FBufferLayout& InLayout) override;

	virtual const FBufferLayout& GetLayout() const override { return Layout; }
private:
	uint32 RendererID = 0;
	FBufferLayout Layout;
};

class FOpenGLIndexBuffer : public FIndexBuffer
{
public:
	FOpenGLIndexBuffer(const uint32* InIndices, uint32 InCount);
	virtual ~FOpenGLIndexBuffer() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual uint32 GetCount() const override { return Count; }
private:
	uint32 RendererID = 0;
	uint32 Count = 0;
};
