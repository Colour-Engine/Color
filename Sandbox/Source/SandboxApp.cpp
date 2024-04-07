#include "SandboxApp.h"

// >>> Entry Point (main function) included here. <<<
// Include this header in only one translation unit (.cpp file etc.) to avoid duplicate symbol errors.
#include "Core/EntryPoint.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"

TRef<FVertexArray>  VertexArray;
TRef<FVertexBuffer> VertexBuffer;
TRef<FIndexBuffer>  IndexBuffer;

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
	// The code below shouldn't render anything at normal conditions.
	// But if the GPU provides a default shader, it will actually render.
	FRenderCommand::Clear();
	FRenderCommand::DrawIndexed(VertexArray);
}

IMPLEMENT_SPECIFICATION("Sandbox");
IMPLEMENT_APPLICATION(FSandboxApp);
