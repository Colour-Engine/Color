#pragma once

#include "Renderer/Shader.h"
#include "GLType.h"

// We don't have our own custom TMap... yet.
// Every instance of the STL map/unordered map will be replaced when TMap gets implemented.
#include <unordered_map>

class FOpenGLShader : public FShader
{
public:
	FOpenGLShader(const FString& Filepath);
	FOpenGLShader(const FString& Name, const FString& VertexSrc, const FString& FragmentSrc);
	~FOpenGLShader() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual const FString& GetName() const override { return Name; }
	virtual const FString& GetFilepath() const override { return Filepath; }
private:
	void PreProcess(const FString& Source);

	void CompileOrGetVulkanBinaries();
	void CompileOrGetOpenGLBinaries();
	void CreateProgram();
	void Reflect(GLenum Stage, const TArray<uint32>& ShaderData);
private:
	uint32 RendererID = 0;
	FString Name, Filepath;

	std::unordered_map<GLenum, TArray<uint32>> VulkanSPIRV;
	std::unordered_map<GLenum, TArray<uint32>> OpenGLSPIRV;

	// Source files (unedited) in string form, mapped to their respective shader types.
	std::unordered_map<GLenum, FString> Sources;
};
