#pragma once

#include "Core/Base.h"

class FShader
{
public:
	virtual ~FShader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual const FString& GetName() const = 0;
	virtual const FString& GetFilepath() const = 0;

	static TRef<FShader> New(const FString& Filepath);
	static TRef<FShader> New(const FString& Name, const FString& VertexSrc, const FString& FragmentSrc);
};
