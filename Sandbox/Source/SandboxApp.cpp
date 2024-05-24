#include "SandboxApp.h"

// >>> Entry Point (main function) included here. <<<
// Include this header in only one translation unit (.cpp file etc.) to avoid duplicate symbol errors.
#include "Core/EntryPoint.h"

#include "Components/TransformComponent.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/UniformBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

TRef<FUniformBuffer> UniformBuffer;
TRef<FVertexArray>   VertexArray;
TRef<FVertexBuffer>  VertexBuffer;
TRef<FIndexBuffer>   IndexBuffer;
TRef<FTexture2D>     Texture;
TRef<FShader>        Shader;

FSandboxApp::FSandboxApp(const FCommandLine& InCommandLine)
	: FApplication(InCommandLine)
{
	float Vertices[] =
	{
		 0.5f,  0.5f, 0.0f,    1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,    1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,    0.0f, 1.0f
	};

	uint32 Indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	Texture = FTexture2D::New("Content/Textures/Engine_Logo_Wide.png");
	Texture->Bind(0);

	int SamplerID = 1;
	UniformBuffer = FUniformBuffer::New(sizeof(int), 0);
	UniformBuffer->SetData(&SamplerID, sizeof(int));

	Shader = FShader::New("Content/Shaders/Textured.glsl");
	Shader->Bind();

	VertexArray = FVertexArray::New();
	VertexArray->Bind();

	VertexBuffer = FVertexBuffer::New(Vertices, sizeof(Vertices));
	VertexBuffer->SetLayout
	({
		{ EShaderDataType::Float3, "a_Position" },
		{ EShaderDataType::Float2, "a_TexCoord" }
	});

	IndexBuffer = FIndexBuffer::New(Indices, 6);
	IndexBuffer->Bind();

	VertexArray->AddVertexBuffer(VertexBuffer);
	VertexArray->SetIndexBuffer(IndexBuffer);

	Sahne = MakeScope<FScene>();

	FEntity Entity = Sahne->CreateEntity("Guten Tag");
	FTransformComponent& Component = Entity.AddOrReplaceComponent<FTransformComponent>(glm::vec2(2.0f, 3.0f));
	FEntity Duplicated = Sahne->DuplicateEntity(Entity);

	CL_CORE_WARN("%f %f", Component.GetLocation2D().x, Component.GetLocation2D().y);
	Sahne->DestroyEntity(Entity);
	CL_CORE_WARN("%f %f", Component.GetLocation2D().x, Component.GetLocation2D().y); // should be gibberish since memory was freed

	Component = Duplicated.GetComponent<FTransformComponent>();
	CL_CORE_WARN("%f %f", Component.GetLocation2D().x, Component.GetLocation2D().y); // this shouldn't as it was duplicated
}

FSandboxApp::~FSandboxApp()
{
}

void FSandboxApp::OnPreAppTick()
{
	// tick test
	Sahne->TickScene(0.0f);

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
