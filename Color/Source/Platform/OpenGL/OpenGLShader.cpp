#include "ColorPCH.h"
#include "OpenGLShader.h"

#include "Utils/FileSystem.h"
#include "Utils/Timer.h"
#include "Misc/Paths.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <fstream>

namespace Utils
{
	static GLenum ShaderTypeFromString(const FString& Type)
	{
		if (Type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (Type == "fragment" || Type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}
	
		unreachable();
		return 0;
	}
	
	static shaderc_shader_kind OpenGLShaderStageToShaderC(GLenum Stage)
	{
		switch (Stage)
		{
		case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
		case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		}

		unreachable();
		return (shaderc_shader_kind) 0;
	}

	static const char* OpenGLShaderStageToString(GLenum Stage)
	{
		switch (Stage)
		{
		case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		}

		unreachable();
		return nullptr;
	}

	static FString GetCacheDirectory()
	{
		return FPaths::GetAPIShaderCacheDirectory("OpenGL");
	}

	static void EnsureCacheDirectoryExists()
	{
		FString Directory = GetCacheDirectory();

		if (!FFileSystem::DoesDirectoryExist(Directory))
		{
			FFileSystem::CreateDirectories(Directory);
		}
	}

	static const char* OpenGLShaderStageCachedGLExtension(GLenum Stage)
	{
		switch (Stage)
		{
		case GL_VERTEX_SHADER: return ".cached_opengl.vert";
		case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";
		}

		unreachable();
		return nullptr;
	}

	static const char* OpenGLShaderStageCachedVKExtension(GLenum Stage)
	{
		switch (Stage)
		{
		case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
		case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";
		}

		unreachable();
		return nullptr;
	}
}

FOpenGLShader::FOpenGLShader(const FString& Filepath)
	: Filepath(Filepath)
{
	Utils::EnsureCacheDirectoryExists();

	FString Source;
	bool bReadSuccess = FFileSystem::ReadFile(Filepath, Source);

	if (!bReadSuccess)
	{
		CL_CORE_ERROR("Couldn't read shader file '%s'!", *Filepath);
		return;
	}
	PreProcess(Source);

	uint_t LastSlash = Filepath.FindLastOf("/\\");
		   LastSlash = LastSlash == FString::Npos ? 0 : LastSlash+1;

	uint_t LastDot = Filepath.Rfind('.');
	uint_t Count = LastDot == FString::Npos ? Filepath.Len() - LastSlash : LastDot - LastSlash;
	Name = Filepath.Sub(LastSlash, Count);

	{
		FTimer Timer;

		CompileOrGetVulkanBinaries();
		CompileOrGetOpenGLBinaries();
		CreateProgram();

		float ElapsedMs = Timer.ElapsedMs();
		CL_CORE_WARN("Shader '%s' creation took '%f' ms.", *Name, ElapsedMs);
	}
}

FOpenGLShader::FOpenGLShader(const FString& Name, const FString& VertexSrc, const FString& FragmentSrc)
	: Name(Name)
{
	Sources[GL_VERTEX_SHADER] = VertexSrc;
	Sources[GL_FRAGMENT_SHADER] = FragmentSrc;

	CompileOrGetVulkanBinaries();
	CompileOrGetOpenGLBinaries();
	CreateProgram();
}

FOpenGLShader::~FOpenGLShader()
{
	glDeleteProgram(RendererID);
}

void FOpenGLShader::Bind() const
{
	glUseProgram(RendererID);
}

void FOpenGLShader::Unbind() const
{
	glUseProgram(0);
}

void FOpenGLShader::PreProcess(const FString& Source)
{
	static const char* TypeToken = "#type";
	static uint_t TypeTokenLength = FStringUtility::Len(TypeToken);

	uint_t Pos = Source.Find(TypeToken);
	while (Pos != FString::Npos)
	{
		uint_t LineEnd = Source.FindFirstOf("\r\n", Pos);
		verifyf(LineEnd != FString::Npos, "Shader type declaration line lacks an end line!");
		uint_t DeclBegin = Pos + TypeTokenLength + 1;
		FString Type = Source.Sub(DeclBegin, LineEnd - DeclBegin);
		verifyf(Utils::ShaderTypeFromString(Type), "Unknown shader type '%s'!", *Type);

		uint_t NextLinePos = Source.FindFirstNotOf("\r\n", LineEnd);
		verifyf(NextLinePos != FString::Npos, "Syntax error!");
		Pos = Source.Find(TypeToken, NextLinePos);

		Sources[Utils::ShaderTypeFromString(Type)] = (Pos == FString::Npos) ? Source.Sub(NextLinePos) : Source.Sub(NextLinePos, Pos - NextLinePos);
	}
}

void FOpenGLShader::CompileOrGetVulkanBinaries()
{
	shaderc::Compiler Compiler;
	shaderc::CompileOptions Options;

	Options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
	Options.SetOptimizationLevel(shaderc_optimization_level_performance);

	FString CacheDirectory = Utils::GetCacheDirectory();
	auto& ShaderData = VulkanSPIRV;
	ShaderData.clear();

	for (auto&& [Stage, Source] : Sources)
	{
		FString CachedPath = CacheDirectory + '/' + (FFileSystem::ExtractFilenameFromFilepath(Filepath) + Utils::OpenGLShaderStageCachedVKExtension(Stage));
		std::ifstream File(*CachedPath, std::ios::binary | std::ios::in);
		
		if (File.is_open())
		{
			File.seekg(0, std::ios::end);
			std::streamsize Size = File.tellg();
			File.seekg(0, std::ios::beg);

			TArray<uint32>& Data = ShaderData[Stage];
			Data.Resize(Size / sizeof(uint32));

			File.read((char*) Data.Get(), Size);
			File.close();
		}
		else
		{
			shaderc::SpvCompilationResult Module = Compiler.CompileGlslToSpv((char*) Source.Get(), Utils::OpenGLShaderStageToShaderC(Stage), *Filepath, Options);

			if (Module.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				CL_CORE_ERROR("%s", Module.GetErrorMessage().c_str());
				check(false);
			}
			
			TArray<uint32>& Data = ShaderData[Stage];
			Data = TArray<uint32>(Module.cbegin(), Module.cend());
			
			std::ofstream Out(*CachedPath, std::ios::binary | std::ios::out);
			if (Out.is_open())
			{
				Out.write((char*) Data.Get(), Data.Num() * sizeof(uint32));
				Out.flush();
				Out.close();
			}
		}
	}

	for (auto&& [Stage, Data] : ShaderData)
	{
		Reflect(Stage, Data);
	}
}

void FOpenGLShader::CompileOrGetOpenGLBinaries()
{
	shaderc::Compiler Compiler;

	shaderc::CompileOptions Options;
	Options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

	FString CacheDirectory = Utils::GetCacheDirectory();
	auto& ShaderData = OpenGLSPIRV;
	ShaderData.clear();
	Sources.clear();

	for (auto&& [Stage, SPIRV] : VulkanSPIRV)
	{
		FString CachedPath = CacheDirectory + '/' + (FFileSystem::ExtractFilenameFromFilepath(Filepath) + Utils::OpenGLShaderStageCachedGLExtension(Stage));
		std::ifstream File(*CachedPath, std::ios::binary | std::ios::in);

		if (File.is_open())
		{
			File.seekg(0, std::ios::end);
			std::streamsize Size = File.tellg();
			File.seekg(0, std::ios::beg);

			TArray<uint32>& Data = ShaderData[Stage];
			Data.Resize(Size / sizeof(uint32));

			File.read((char*)Data.Get(), Size);
			File.close();
		}
		else
		{
			spirv_cross::CompilerGLSL GLSLCompiler(SPIRV.Get(), SPIRV.Num());
			Sources[Stage] = GLSLCompiler.compile().c_str();
			FString& Source = Sources[Stage];

			shaderc::SpvCompilationResult Module = Compiler.CompileGlslToSpv((char*) Source.Get(), Utils::OpenGLShaderStageToShaderC(Stage), *Filepath);

			if (Module.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				CL_CORE_ERROR("%s", Module.GetErrorMessage().c_str());
				check(false);
			}

			TArray<uint32>& Data = ShaderData[Stage];
			Data = TArray<uint32>(Module.cbegin(), Module.cend());

			std::ofstream Out(*CachedPath, std::ios::binary | std::ios::out);
			if (Out.is_open())
			{
				Out.write((char*)Data.Get(), Data.Num() * sizeof(uint32));
				Out.flush();
				Out.close();
			}
		}
	}
}

void FOpenGLShader::CreateProgram()
{
	GLuint Program = glCreateProgram();
	uint32 ShaderIDs[2] = { 0, 0 };

	{
		uint32 i = 0;
		for (auto&& [Stage, SPIRV] : OpenGLSPIRV)
		{
			uint32 ShaderID = ShaderIDs[i++] =
			glCreateShader(Stage);
			glShaderBinary(1, &ShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, SPIRV.Get(), (GLsizei)(SPIRV.Num() * sizeof(uint32)));
			glSpecializeShader(ShaderID, "main", 0, nullptr, nullptr);
			glAttachShader(Program, ShaderID);
		}
	}
	glLinkProgram(Program);

	GLint LinkResult;
	glGetProgramiv(Program, GL_LINK_STATUS, &LinkResult);

	if (LinkResult != GL_TRUE)
	{
		GLint InfoLogLength;
		glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &InfoLogLength);

		FString InfoLog('\0', InfoLogLength);
		glGetProgramInfoLog(Program, InfoLogLength, &InfoLogLength, InfoLog.Get());

		CL_CORE_ERROR("Shader linking failure for '%s': %s", *Filepath, *InfoLog);
		for (uint32 ID : ShaderIDs)
		{
			glDeleteShader(ID);
		}
		glDeleteProgram(Program);

		return;
	}

	for (uint32 ID : ShaderIDs)
	{
		glDetachShader(Program, ID);
		glDeleteShader(ID);
	}

	RendererID = Program;
}

void FOpenGLShader::Reflect(GLenum Stage, const TArray<uint32>& ShaderData)
{
	spirv_cross::Compiler Compiler(ShaderData.Get(), ShaderData.Num());
	spirv_cross::ShaderResources Resources = Compiler.get_shader_resources();

	CL_CORE_TRACE("FOpenGLShader::Reflect - %s %s [%s]", Utils::OpenGLShaderStageToString(Stage), *Name, *Filepath);
	CL_CORE_TRACE("  %d uniform buffers", Resources.uniform_buffers.size());
	CL_CORE_TRACE("  %d resources", Resources.sampled_images.size());

	CL_CORE_TRACE("Uniform buffers:");
	for (const spirv_cross::Resource& Resource : Resources.uniform_buffers)
	{
		const spirv_cross::SPIRType& BufferType = Compiler.get_type(Resource.base_type_id);
		uint_t BufferSize = Compiler.get_declared_struct_size(BufferType);
		uint32 Binding = Compiler.get_decoration(Resource.id, spv::DecorationBinding);
		uint_t MemberCount = BufferType.member_types.size();

		CL_CORE_TRACE("  %s", Resource.name.c_str());
		CL_CORE_TRACE("  Size = %d", BufferSize);
		CL_CORE_TRACE("  Binding = %d", Binding);
		CL_CORE_TRACE("  No. Members = %d", MemberCount);
	}
}
