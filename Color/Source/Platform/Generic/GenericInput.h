#pragma once

#include "Input/Input.h"

class FGenericInput : public FInput
{
protected:
	virtual bool IsKeyPressed_Impl(EKeyCode KeyCode) override;
	virtual bool IsMouseButtonPressed_Impl(EMouseCode Button) override;

	virtual glm::vec2 GetMousePosition_Impl() override;
};
