#include "Sandbox2D.h"

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

void FSandbox2D::OnAttach()
{
	Scene = MakeScope<FScene>();

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

	FEntity Entity = Scene->CreateEntity("Guten Tag");
	FTransformComponent& Component = Entity.AddOrReplaceComponent<FTransformComponent>(glm::vec2(2.0f, 3.0f));
	FEntity Duplicated = Scene->DuplicateEntity(Entity);
}

void FSandbox2D::OnTick(float DeltaTime)
{
	Scene->TickScene(DeltaTime);

	FRenderCommand::Clear();
	FRenderCommand::DrawIndexed(VertexArray);
}

void FSandbox2D::OnDetach()
{
	UniformBuffer.Reset();
	VertexArray.Reset();
	VertexBuffer.Reset();
	IndexBuffer.Reset();
	Texture.Reset();
	Shader.Reset();
}
