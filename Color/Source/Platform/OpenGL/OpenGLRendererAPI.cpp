#include "ColorPCH.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

static void GLMessageCallback
(
	unsigned    Src,
	unsigned    Type,
	unsigned    ID,
	unsigned    Severity,
	int         Length,
	const char* Msg,
	const void* UsrParam
)
{
	switch (Severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         CL_CORE_FATAL(Msg); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       CL_CORE_ERROR(Msg); break;
	case GL_DEBUG_SEVERITY_LOW:          CL_CORE_WARN(Msg); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: CL_CORE_TRACE(Msg); break;
	}

	unreachable();
}

void FOpenGLRendererAPI::Init()
{
#ifndef CL_SHIPPING
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	glDebugMessageCallback(GLMessageCallback, nullptr);
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
}

void FOpenGLRendererAPI::SetViewport(uint32 X, uint32 Y, uint32 Width, uint32 Height)
{
	glViewport(X, Y, Width, Height);
}

void FOpenGLRendererAPI::SetClearColor(const glm::vec4& Color)
{
	glClearColor(Color.x, Color.y, Color.z, Color.w);
}

void FOpenGLRendererAPI::SetLineWidth(float Width)
{
	glLineWidth(Width);
}

void FOpenGLRendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
