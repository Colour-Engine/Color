#pragma once

#include "Core/Base.h"

enum class EShaderDataType
{
	None = 0,
	Float, Float2, Float3, Float4,
	Mat3, Mat4,
	Int, Int2, Int3, Int4,
	Bool
};

uint32 SizeOfShaderDataType(EShaderDataType Type);

struct FBufferElement
{
	FString Name;
	EShaderDataType Type = EShaderDataType::None;
	uint32 Size = 0;
	uint_t Offset = 0;
	bool bNormalized = false;

	FBufferElement() = default;
	FBufferElement(EShaderDataType Type, const FString& Name, bool bNormalized = false)
		: Type(Type), Name(Name), Size(SizeOfShaderDataType(Type)), bNormalized(bNormalized)
	{
	}

	uint8 GetComponentCount() const
	{
		switch (Type)
		{
		case EShaderDataType::Float:  return 1;
		case EShaderDataType::Float2: return 2;
		case EShaderDataType::Float3: return 3;
		case EShaderDataType::Float4: return 4;
		case EShaderDataType::Mat3:   return 3;
		case EShaderDataType::Mat4:   return 4;
		case EShaderDataType::Int:    return 1;
		case EShaderDataType::Int2:   return 2;
		case EShaderDataType::Int3:   return 3;
		case EShaderDataType::Int4:   return 4;
		case EShaderDataType::Bool:   return 1;
		}

		unreachable();
		return 0;
	}
};

class FBufferLayout
{
public:
	FBufferLayout() = default;
	FBufferLayout(std::initializer_list<FBufferElement> InElements)
		: Elements(InElements)
	{
		CalculateOffsetsAndStride();
	}

	const TArray<FBufferElement>& GetElements() const { return Elements; }
	uint32 GetStride() const { return Stride; }

	TArray<FBufferElement>::Iterator begin() { return Elements.begin(); }
	TArray<FBufferElement>::Iterator end() { return Elements.end(); }
	TArray<FBufferElement>::ConstIterator begin() const { return Elements.begin(); }
	TArray<FBufferElement>::ConstIterator end() const { return Elements.end(); }
public:
	void CalculateOffsetsAndStride()
	{
		uint_t Offset = 0;
		Stride = 0;

		for (FBufferElement& Element : Elements)
		{
			Element.Offset = Offset;
			Offset += Element.Size;
			Stride += Element.Size;
		}
	}
private:
	TArray<FBufferElement> Elements;
	uint32 Stride = 0;
};

class FVertexBuffer
{
public:
	virtual ~FVertexBuffer() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetData(const void* InData, uint32 InSize) = 0;
	virtual void SetLayout(const FBufferLayout& InLayout) = 0;

	virtual const FBufferLayout& GetLayout() const = 0;

	static TRef<FVertexBuffer> New(uint32 InSize);
	static TRef<FVertexBuffer> New(const float* InVertices, uint32 InSize);
};

class FIndexBuffer
{
public:
	virtual ~FIndexBuffer() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual uint32 GetCount() const = 0;

	static TRef<FIndexBuffer> New(const uint32* InIndices, uint32 InCount);
};
