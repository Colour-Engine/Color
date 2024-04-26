#include "SandboxApp.h"

// >>> Entry Point (main function) included here. <<<
// Include this header in only one translation unit (.cpp file etc.) to avoid duplicate symbol errors.
#include "Core/EntryPoint.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

TRef<FVertexArray>  VertexArray;
TRef<FVertexBuffer> VertexBuffer;
TRef<FIndexBuffer>  IndexBuffer;
TRef<FShader>       Shader;

FSandboxApp::FSandboxApp(const FCommandLine& InCommandLine)
	: FApplication(InCommandLine)
{
	float Vertices[] =
	{
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	uint32 Indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	Shader = FShader::New("Content/Shaders/FlatColor.glsl");
	Shader->Bind();

	VertexArray = FVertexArray::New();
	VertexArray->Bind();

	VertexBuffer = FVertexBuffer::New(Vertices, sizeof(Vertices));
	VertexBuffer->SetLayout
	({
		{ EShaderDataType::Float3, "a_Position" }
	});

	IndexBuffer = FIndexBuffer::New(Indices, 6);
	IndexBuffer->Bind();

	VertexArray->AddVertexBuffer(VertexBuffer);
	VertexArray->SetIndexBuffer(IndexBuffer);
}

FSandboxApp::~FSandboxApp()
{
}

void FSandboxApp::OnPreAppTick()
{
	FRenderCommand::Clear();
	FRenderCommand::DrawIndexed(VertexArray);
}

IMPLEMENT_SPECIFICATION
(
	"Sandbox", // Application name
	".",       // Working directory
	{          // Window properties
		"Sandbox", // Title
		 1600,     // Width
		 900,      // Height
		 false,    // Fullscreen?
		 false,    // Resizable?
		 true      // VSync?
	}
);

IMPLEMENT_APPLICATION(FSandboxApp);
