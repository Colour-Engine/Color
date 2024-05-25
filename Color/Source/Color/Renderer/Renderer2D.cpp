#include "ColorPCH.h"
#include "Renderer2D.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/UniformBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/RenderCaps.h"
#include "Renderer/Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct FQuadVertex
{
	glm::vec3 Location;
	glm::vec4 Color;
	glm::vec2 TexCoord;

	int TexIndex = 0;
	float TilingFactor = 0.0f;

	int EntityID = 0;
};

struct FCircleVertex
{
	glm::vec3 WorldLocation;
	glm::vec3 LocalLocation;
	glm::vec4 Color;

	float Thickness = 0.0f;
	float Fade = 0.0f;

	int EntityID = 0;
};

struct FLineVertex
{
	glm::vec3 Location;
	glm::vec4 Color;

	int EntityID = 0;
};

struct FRenderer2DData
{
	TRef<FVertexArray> QuadVertexArray;
	TRef<FVertexBuffer> QuadVertexBuffer;
	TRef<FShader> QuadShader;
	TRef<FTexture2D> DefaultTexture;

	TRef<FVertexArray> CircleVertexArray;
	TRef<FVertexBuffer> CircleVertexBuffer;
	TRef<FShader> CircleShader;

	TRef<FVertexArray> LineVertexArray;
	TRef<FVertexBuffer> LineVertexBuffer;
	TRef<FShader> LineShader;

	uint32 QuadIndexCount = 0;
	FQuadVertex* QuadVertexBufferBase = nullptr;
	FQuadVertex* QuadVertexBufferPtr = nullptr;

	uint32 CircleIndexCount = 0;
	FCircleVertex* CircleVertexBufferBase = nullptr;
	FCircleVertex* CircleVertexBufferPtr = nullptr;

	uint32 LineVertexCount = 0;
	FLineVertex* LineVertexBufferBase = nullptr;
	FLineVertex* LineVertexBufferPtr = nullptr;

	float LineWidth = 2.0f;

	// Guaranteed to have FRenderCaps::MaxTextureSlots amount of elements.
	TArray<TRef<FTexture2D>> TextureSlots;
	uint32 TextureSlotIndex = 1; // 0 is reserved for the DefaultTexture.

	glm::vec4 QuadVertexLocations[4] =
	{
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f },
	};

	glm::vec2 TextureCoords[4] =
	{
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};

	FRenderer2D::FStatistics Statistics;

	struct FCameraData
	{
		glm::mat4 ViewProjection;
	} CameraBuffer;

	TRef<FUniformBuffer> CameraUniformBuffer;
};

static FRenderer2DData Data;

void FRenderer2D::Init()
{
	Data.QuadVertexArray = FVertexArray::New();
	Data.QuadVertexBuffer = FVertexBuffer::New(FRenderCaps::MaxVertices * sizeof(FQuadVertex));
	Data.QuadVertexBuffer->SetLayout({
		{ EShaderDataType::Float3, "a_Location" },
		{ EShaderDataType::Float4, "a_Color" },
		{ EShaderDataType::Float2, "a_TexCoord" },
		{ EShaderDataType::Int,    "a_TexIndex" },
		{ EShaderDataType::Float,  "a_TilingFactor" },
		{ EShaderDataType::Int,    "a_EntityID" }
	});
	Data.QuadVertexArray->AddVertexBuffer(Data.QuadVertexBuffer);
	Data.QuadVertexBufferBase = new FQuadVertex[FRenderCaps::MaxVertices];

	uint32* QuadIndices = new uint32[FRenderCaps::MaxIndices];
	uint32  Offset = 0;

	for (uint32 i = 0; i < FRenderCaps::MaxIndices; i += 6)
	{
		QuadIndices[  i  ] = Offset;
		QuadIndices[i + 1] = Offset + 1;
		QuadIndices[i + 2] = Offset + 2;

		QuadIndices[i + 3] = Offset + 2;
		QuadIndices[i + 4] = Offset + 3;
		QuadIndices[i + 5] = Offset;

		Offset += 4;
	}

	TRef<FIndexBuffer> QuadIB = FIndexBuffer::New(QuadIndices, FRenderCaps::MaxIndices);
	Data.QuadVertexArray->SetIndexBuffer(QuadIB);
	delete[] QuadIndices;

	Data.CircleVertexArray = FVertexArray::New();
	Data.CircleVertexBuffer = FVertexBuffer::New(FRenderCaps::MaxVertices * sizeof(FCircleVertex));
	Data.CircleVertexBuffer->SetLayout({
		{ EShaderDataType::Float3, "a_WorldLocation" },
		{ EShaderDataType::Float3, "a_LocalLocation" },
		{ EShaderDataType::Float4, "a_Color" },
		{ EShaderDataType::Float,  "a_Thickness" },
		{ EShaderDataType::Float,  "a_Fade" },
		{ EShaderDataType::Int,    "a_EntityID" }
	});
	Data.CircleVertexArray->AddVertexBuffer(Data.CircleVertexBuffer);
	Data.CircleVertexArray->SetIndexBuffer(QuadIB);
	Data.CircleVertexBufferBase = new FCircleVertex[FRenderCaps::MaxVertices];

	Data.LineVertexArray = FVertexArray::New();
	Data.LineVertexBuffer = FVertexBuffer::New(FRenderCaps::MaxVertices * sizeof(FLineVertex));
	Data.LineVertexBuffer->SetLayout({
		{ EShaderDataType::Float3, "a_Location" },
		{ EShaderDataType::Float4, "a_Color" },
		{ EShaderDataType::Int,    "a_EntityID" }
	});
	Data.LineVertexArray->AddVertexBuffer(Data.LineVertexBuffer);
	Data.LineVertexBufferBase = new FLineVertex[FRenderCaps::MaxVertices];

	Data.DefaultTexture = FTexture2D::New(FTextureSpecification{});
	uint32 DefaultTextureData = 0xffffffff;
	Data.DefaultTexture->SetData(&DefaultTextureData, sizeof(uint32));

	Data.QuadShader = FShader::New("Content/Shaders/Renderer2D_Quad.glsl");
	Data.CircleShader = FShader::New("Content/Shaders/Renderer2D_Circle.glsl");
	Data.LineShader = FShader::New("Content/Shaders/Renderer2D_Line.glsl");

	Data.TextureSlots.Resize(FRenderCaps::MaxTextureSlots);
	Data.TextureSlots[0] = Data.DefaultTexture;
	Data.TextureSlotIndex = 1;

	Data.CameraUniformBuffer = FUniformBuffer::New(sizeof(FRenderer2DData::FCameraData), 0);
}

void FRenderer2D::Shutdown()
{
	Data.QuadVertexArray.Reset();
	Data.QuadVertexBuffer.Reset();
	Data.QuadShader.Reset();

	Data.CircleVertexArray.Reset();
	Data.CircleVertexBuffer.Reset();
	Data.CircleShader.Reset();

	Data.LineVertexArray.Reset();
	Data.LineVertexBuffer.Reset();
	Data.LineShader.Reset();

	Data.DefaultTexture.Reset();
	Data.CameraUniformBuffer.Reset();

	delete[] Data.QuadVertexBufferBase;
	delete[] Data.CircleVertexBufferBase;
	delete[] Data.LineVertexBufferBase;

	Data.QuadVertexBufferBase = Data.QuadVertexBufferPtr = nullptr;
	Data.CircleVertexBufferBase = Data.CircleVertexBufferPtr = nullptr;
	Data.LineVertexBufferBase = Data.LineVertexBufferPtr = nullptr;

	Data.TextureSlots.Clear();
	Data.TextureSlotIndex = 0;
}

void FRenderer2D::BeginScene(const FCamera& Camera, const glm::mat4& Transform)
{
	Data.CameraBuffer.ViewProjection = Camera.GetProjection() * glm::inverse(Transform);
	Data.CameraUniformBuffer->SetData(&Data.CameraBuffer, sizeof(FRenderer2DData::FCameraData));

	StartBatch();
}

void FRenderer2D::EndScene()
{
	Flush();
}

void FRenderer2D::StartBatch()
{
	Data.QuadIndexCount = 0;
	Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;

	Data.CircleIndexCount = 0;
	Data.CircleVertexBufferPtr = Data.CircleVertexBufferBase;

	Data.LineVertexCount = 0;
	Data.LineVertexBufferPtr = Data.LineVertexBufferBase;

	Data.TextureSlotIndex = 1;
}

void FRenderer2D::Flush()
{
	if (Data.QuadIndexCount > 0)
	{
		uint32 DataSize = (uint32_t)((uint8_t*) Data.QuadVertexBufferPtr - (uint8_t*) Data.QuadVertexBufferBase);
		Data.QuadVertexBuffer->SetData(Data.QuadVertexBufferBase, DataSize);

		for (uint32_t i = 0; i < Data.TextureSlotIndex; i++)
		{
			Data.TextureSlots[i]->Bind(i);
		}

		Data.QuadShader->Bind();
		FRenderCommand::DrawIndexed(Data.QuadVertexArray, Data.QuadIndexCount);
		Data.Statistics.DrawCalls++;
	}

	if (Data.CircleIndexCount > 0)
	{
		uint32 DataSize = (uint32_t)((uint8_t*) Data.CircleVertexBufferPtr - (uint8_t*) Data.CircleVertexBufferBase);
		Data.CircleVertexBuffer->SetData(Data.CircleVertexBufferBase, DataSize);
		
		Data.CircleShader->Bind();
		FRenderCommand::DrawIndexed(Data.CircleVertexArray, Data.CircleIndexCount);
		Data.Statistics.DrawCalls++;
	}

	if (Data.LineVertexCount > 0)
	{
		uint32 DataSize = (uint32_t)((uint8_t*) Data.LineVertexBufferPtr - (uint8_t*) Data.LineVertexBufferBase);
		Data.LineVertexBuffer->SetData(Data.LineVertexBufferBase, DataSize);
		
		Data.LineShader->Bind();
		FRenderCommand::SetLineWidth(Data.LineWidth);
		FRenderCommand::DrawLines(Data.LineVertexArray, Data.LineVertexCount);
		Data.Statistics.DrawCalls++;
	}
}

void FRenderer2D::NextBatch()
{
	Flush();
	StartBatch();
}

void FRenderer2D::DrawQuad(const glm::vec2& Location, const glm::vec2& Scale, const glm::vec4& Color)
{
	DrawQuad({ Location.x, Location.y, 0.0f }, Scale, Color);
}

void FRenderer2D::DrawQuad(const glm::vec3& Location, const glm::vec2& Scale, const glm::vec4& Color)
{
	glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Location) * glm::scale(glm::mat4(1.0f), { Scale.x, Scale.y, 1.0f });
	DrawQuad(Transform, Color);
}

void FRenderer2D::DrawQuad(const glm::vec2& Location, const glm::vec2& Scale, const TRef<FTexture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
{
	DrawQuad({ Location.x, Location.y, 0.0f }, Scale, Texture, TilingFactor, TintColor);
}

void FRenderer2D::DrawQuad(const glm::vec3& Location, const glm::vec2& Scale, const TRef<FTexture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
{
	glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Location) * glm::scale(glm::mat4(1.0f), { Scale.x, Scale.y, 1.0f });
	DrawQuad(Transform, Texture, TilingFactor, TintColor);
}

void FRenderer2D::DrawQuad(const glm::mat4& Transform, const glm::vec4& Color, int EntityID)
{
	if (Data.QuadIndexCount >= FRenderCaps::MaxIndices)
	{
		NextBatch();
	}

	for (uint_t i = 0; i < 4; i++)
	{
		Data.QuadVertexBufferPtr->Location = Transform * Data.QuadVertexLocations[i];
		Data.QuadVertexBufferPtr->Color = Color;
		Data.QuadVertexBufferPtr->TexCoord = Data.TextureCoords[i];
		Data.QuadVertexBufferPtr->TexIndex = 0; // Default Texture
		Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		Data.QuadVertexBufferPtr->EntityID = EntityID;
		Data.QuadVertexBufferPtr++;
	}

	Data.QuadIndexCount += 6;
	Data.Statistics.QuadCount++;
}

void FRenderer2D::DrawQuad(const glm::mat4& Transform, const TRef<FTexture2D>& Texture, float TilingFactor, const glm::vec4& TintColor, int EntityID)
{
	if (Data.QuadIndexCount >= FRenderCaps::MaxIndices)
	{
		NextBatch();
	}

	int32 TextureIndex = 0;
	for (uint32 i = 0; i < Data.TextureSlotIndex; i++)
	{
		if (*Data.TextureSlots[i] == *Texture)
		{
			TextureIndex = i;
			break;
		}
	}

	if (TextureIndex == 0)
	{
		if (Data.TextureSlotIndex >= FRenderCaps::MaxTextureSlots)
		{
			NextBatch();
		}

		TextureIndex = Data.TextureSlotIndex;
		Data.TextureSlots[TextureIndex] = Texture;
		Data.TextureSlotIndex++;
	}

	for (uint_t i = 0; i < 4; i++)
	{
		Data.QuadVertexBufferPtr->Location = Transform * Data.QuadVertexLocations[i];
		Data.QuadVertexBufferPtr->Color = TintColor;
		Data.QuadVertexBufferPtr->TexCoord = Data.TextureCoords[i];
		Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
		Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		Data.QuadVertexBufferPtr->EntityID = EntityID;
		Data.QuadVertexBufferPtr++;
	}

	Data.QuadIndexCount += 6;
	Data.Statistics.QuadCount++;
}

void FRenderer2D::DrawRotatedQuad(const glm::vec2& Location, const glm::vec2& Scale, float Rotation, const glm::vec4& Color)
{
	DrawRotatedQuad({ Location.x, Location.y, 0.0f }, Scale, Rotation, Color);
}

void FRenderer2D::DrawRotatedQuad(const glm::vec3& Location, const glm::vec2& Scale, float Rotation, const glm::vec4& Color)
{
	glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Location) *
						  glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), { 0.0f, 0.0f, 1.0f }) *
						  glm::scale(glm::mat4(1.0f), { Scale.x, Scale.y, 1.0f });

	DrawQuad(Transform, Color);
}

void FRenderer2D::DrawRotatedQuad(const glm::vec2& Location, const glm::vec2& Scale, float Rotation, const TRef<FTexture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
{
	DrawRotatedQuad({ Location.x, Location.y, 0.0f }, Scale, Rotation, Texture, TilingFactor, TintColor);
}

void FRenderer2D::DrawRotatedQuad(const glm::vec3& Location, const glm::vec2& Scale, float Rotation, const TRef<FTexture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
{
	glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Location) *
						  glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), { 0.0f, 0.0f, 1.0f }) *
						  glm::scale(glm::mat4(1.0f), { Scale.x, Scale.y, 1.0f });

	DrawQuad(Transform, Texture, TilingFactor, TintColor);
}

void FRenderer2D::DrawCircle(const glm::mat4& Transform, const glm::vec4& Color, float Thickness, float Fade, int EntityID)
{
	if (Data.CircleIndexCount >= FRenderCaps::MaxIndices)
	{
		NextBatch();
	}

	for (uint32 i = 0; i < 4; i++)
	{
		Data.CircleVertexBufferPtr->WorldLocation = Transform * Data.QuadVertexLocations[i];
		Data.CircleVertexBufferPtr->LocalLocation = Data.QuadVertexLocations[i] * 2.0f;
		Data.CircleVertexBufferPtr->Color = Color;
		Data.CircleVertexBufferPtr->Thickness = Thickness;
		Data.CircleVertexBufferPtr->Fade = Fade;
		Data.CircleVertexBufferPtr->EntityID = EntityID;
		Data.CircleVertexBufferPtr++;
	}

	Data.CircleIndexCount += 6;
	Data.Statistics.QuadCount++;
}

void FRenderer2D::DrawLine(const glm::vec3& Start, const glm::vec3& End, const glm::vec4& Color, int EntityID)
{
	if (Data.LineVertexCount >= FRenderCaps::MaxVertices)
	{
		NextBatch();
	}

	Data.LineVertexBufferPtr->Location = Start;
	Data.LineVertexBufferPtr->Color = Color;
	Data.LineVertexBufferPtr->EntityID = EntityID;
	Data.LineVertexBufferPtr++;

	Data.LineVertexBufferPtr->Location = End;
	Data.LineVertexBufferPtr->Color = Color;
	Data.LineVertexBufferPtr->EntityID = EntityID;
	Data.LineVertexBufferPtr++;

	Data.LineVertexCount += 2;
}

void FRenderer2D::DrawRect(const glm::vec3& Location, const glm::vec2& Scale, const glm::vec4& Color, int EntityID)
{
	glm::vec3 L0 = { Location.x - Scale.x * 0.5f, Location.y - Scale.y * 0.5f, Location.z };
	glm::vec3 L1 = { Location.x + Scale.x * 0.5f, Location.y - Scale.y * 0.5f, Location.z };
	glm::vec3 L2 = { Location.x + Scale.x * 0.5f, Location.y + Scale.y * 0.5f, Location.z };
	glm::vec3 L3 = { Location.x - Scale.x * 0.5f, Location.y + Scale.y * 0.5f, Location.z };

	DrawLine(L0, L1, Color, EntityID);
	DrawLine(L1, L2, Color, EntityID);
	DrawLine(L2, L3, Color, EntityID);
	DrawLine(L3, L0, Color, EntityID);
}

void FRenderer2D::DrawRect(const glm::mat4& Transform, const glm::vec4& Color, int EntityID)
{
	glm::vec3 LineVertices[4];
	for (uint_t i = 0; i < 4; i++)
	{
		LineVertices[i] = Transform * Data.QuadVertexLocations[i];
	}

	DrawLine(LineVertices[0], LineVertices[1], Color, EntityID);
	DrawLine(LineVertices[1], LineVertices[2], Color, EntityID);
	DrawLine(LineVertices[2], LineVertices[3], Color, EntityID);
	DrawLine(LineVertices[3], LineVertices[0], Color, EntityID);
}

void FRenderer2D::DrawSprite(const glm::mat4& Transform, FSpriteRendererComponent& SpriteRendererComponent, int EntityID)
{
	if (SpriteRendererComponent.GetTexture())
	{
		DrawQuad
		(
			Transform,
			SpriteRendererComponent.GetTexture(),
			SpriteRendererComponent.GetTilingFactor(),
			SpriteRendererComponent.GetColor(),
			EntityID
		);
	}
	else
	{
		DrawQuad
		(
			Transform,
			SpriteRendererComponent.GetColor(),
			EntityID
		);
	}
}

void FRenderer2D::DrawCircle(const glm::mat4& Transform, FCircleRendererComponent& CircleRendererComponent, int EntityID)
{
	DrawCircle
	(
		Transform,
		CircleRendererComponent.GetColor(),
		CircleRendererComponent.GetThickness(),
		CircleRendererComponent.GetFade(),
		EntityID
	);
}

float FRenderer2D::GetLineWidth()
{
	return Data.LineWidth++;
}

void FRenderer2D::SetLineWidth(float NewLineWidth)
{
	Data.LineWidth = NewLineWidth;
}

void FRenderer2D::ResetStatistics()
{
	memset(&Data.Statistics, 0, sizeof(FRenderer2D::FStatistics));
}

FRenderer2D::FStatistics FRenderer2D::GetStatistics()
{
	return Data.Statistics;
}
