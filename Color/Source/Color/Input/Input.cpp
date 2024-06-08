#include "ColorPCH.h"
#include "Input.h"

#include "Platform/Windows/WindowsInput.h"
#include "Platform/Linux/LinuxInput.h"

TScope<FInput> FInput::Instance = MakeScope<ConcatWithPlatformNameDef(Input)>();

bool FInput::IsKeyPressed(EKeyCode KeyCode)
{
	return Instance->IsKeyPressed_Impl(KeyCode);
}

bool FInput::IsMouseButtonPressed(EMouseCode Button)
{
	return Instance->IsMouseButtonPressed_Impl(Button);
}

glm::vec2 FInput::GetMousePosition()
{
	return Instance->GetMousePosition_Impl();
}

float FInput::GetMouseX()
{
	return GetMousePosition().x;
}

float FInput::GetMouseY()
{
	return GetMousePosition().y;
}
