#pragma once

#include "Core/Base.h"

#include "Components/SpriteRendererComponent.h"
#include "Components/CircleRendererComponent.h"

#include "Renderer/Camera.h"

class FRenderer2D
{
public:
	static void Init();
	static void Shutdown();

	static void BeginScene(const FCamera& Camera, const glm::mat4& Transform);
	static void EndScene();
	static void Flush();

	static void DrawQuad(const glm::vec2& Location, const glm::vec2& Scale, const glm::vec4& Color);
	static void DrawQuad(const glm::vec3& Location, const glm::vec2& Scale, const glm::vec4& Color);
	static void DrawQuad(const glm::vec2& Location, const glm::vec2& Scale, const TRef<FTexture2D>& Texture, float TilingFactor = 1.0f, const glm::vec4& TintColor = glm::vec4(1.0f));
	static void DrawQuad(const glm::vec3& Location, const glm::vec2& Scale, const TRef<FTexture2D>& Texture, float TilingFactor = 1.0f, const glm::vec4& TintColor = glm::vec4(1.0f));

	static void DrawQuad(const glm::mat4& Transform, const glm::vec4& Color, int EntityID = -1);
	static void DrawQuad(const glm::mat4& Transform, const TRef<FTexture2D>& Texture, float TilingFactor = 1.0f, const glm::vec4& TintColor = glm::vec4(1.0f), int EntityID = -1);

	static void DrawRotatedQuad(const glm::vec2& Location, const glm::vec2& Scale, float Rotation, const glm::vec4& Color);
	static void DrawRotatedQuad(const glm::vec3& Location, const glm::vec2& Scale, float Rotation, const glm::vec4& Color);
	static void DrawRotatedQuad(const glm::vec2& Location, const glm::vec2& Scale, float Rotation, const TRef<FTexture2D>& Texture, float TilingFactor = 1.0f, const glm::vec4& TintColor = glm::vec4(1.0f));
	static void DrawRotatedQuad(const glm::vec3& Location, const glm::vec2& Scale, float Rotation, const TRef<FTexture2D>& Texture, float TilingFactor = 1.0f, const glm::vec4& TintColor = glm::vec4(1.0f));

	static void DrawCircle(const glm::mat4& Transform, const glm::vec4& Color, float Thickness = 1.0f, float Fade = 0.005f, int EntityID = -1);
	static void DrawLine(const glm::vec3& Start, const glm::vec3& End, const glm::vec4& Color, int EntityID = -1);

	static void DrawRect(const glm::vec3& Location, const glm::vec2& Scale, const glm::vec4& Color, int EntityID = -1);
	static void DrawRect(const glm::mat4& Transform, const glm::vec4& Color, int EntityID = -1);

	static void DrawSprite(const glm::mat4& Transform, FSpriteRendererComponent& SpriteRendererComponent, int EntityID);
	static void DrawCircle(const glm::mat4& Transform, FCircleRendererComponent& CircleRendererComponent, int EntityID);

	static float GetLineWidth();
	static void SetLineWidth(float NewLineWidth);
	
	struct FStatistics
	{
		uint32 DrawCalls = 0;
		uint32 QuadCount = 0;

		uint32 GetTotalVertexCount() const { return QuadCount * 4; /*4 vertices per quad*/ }
		uint32 GetTotalIndexCount() const { return QuadCount * 6; /*6 indices per quad*/ }
	};

	static void ResetStatistics();
	static FStatistics GetStatistics();
private:
	static void StartBatch();
	static void NextBatch();
};
