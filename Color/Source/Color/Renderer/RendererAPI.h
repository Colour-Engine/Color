#pragma once

#include "Core/Base.h"

#include <glm/glm.hpp>

enum class ERendererAPI
{
	None = 0,
	OpenGL // OpenGL 4.5/4.6
};

class FRendererAPI
{
public:
	virtual ~FRendererAPI() = default;

	virtual void Init() = 0;
	virtual void SetViewport(uint32 X, uint32 Y, uint32 Width, uint32 Height) = 0;
	virtual void SetClearColor(const glm::vec4& Color) = 0;
	virtual void SetLineWidth(float Width) = 0;
	virtual void Clear() = 0;

	static ERendererAPI GetAPI() { return API; }
	static TScope<FRendererAPI> New();
private:
	// By default, OpenGL (since it's the only available one for now but in the future (when Vulkan etc. gets added) we'll make the selection at runtime.
	inline static ERendererAPI API = ERendererAPI::OpenGL;
};
