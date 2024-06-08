#pragma once

#include "Core/Base.h"

#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"

#include <glm/glm.hpp>

class FInput
{
public:
	virtual ~FInput() = default;

	static bool IsKeyPressed(EKeyCode KeyCode);
	static bool IsMouseButtonPressed(EMouseCode Button);

	static glm::vec2 GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();
protected:
	virtual bool IsKeyPressed_Impl(EKeyCode KeyCode) = 0;
	virtual bool IsMouseButtonPressed_Impl(EMouseCode Button) = 0;

	virtual glm::vec2 GetMousePosition_Impl() = 0;
private:
	static TScope<FInput> Instance;
};
