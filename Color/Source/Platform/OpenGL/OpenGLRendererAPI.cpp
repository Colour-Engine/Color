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

void FOpenGLRendererAPI::SetRenderMode(ERenderMode Mode)
{
	GLenum GLMode = 0x0;

	switch (Mode)
	{
	case ERenderMode::Solid: GLMode = GL_FILL; break;
	case ERenderMode::Wireframe: GLMode = GL_LINE; break;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GLMode);
}

void FOpenGLRendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FOpenGLRendererAPI::DrawIndexed(const TRef<FVertexArray>& VertexArray, uint32 IndexCount)
{
	VertexArray->Bind();
	uint8 Count = IndexCount ? IndexCount : VertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, Count, GL_UNSIGNED_INT, nullptr);
}

void FOpenGLRendererAPI::DrawLines(const TRef<FVertexArray>& VertexArray, uint32 VertexCount)
{
	VertexArray->Bind();
	glDrawArrays(GL_LINES, 0, VertexCount);
}
