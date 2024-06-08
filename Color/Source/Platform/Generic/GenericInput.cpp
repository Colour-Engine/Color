#include "ColorPCH.h"
#include "GenericInput.h"

#include "Core/Application.h"
#include <GLFW/glfw3.h>

bool FGenericInput::IsKeyPressed_Impl(EKeyCode KeyCode)
{
	GLFWwindow* Window = (GLFWwindow*) FApplication::Get()->GetWindow()->GetNativeWindowHandle();
	int Status = glfwGetKey(Window, KeyCode);
	return Status == GLFW_PRESS || Status == GLFW_REPEAT;
}

bool FGenericInput::IsMouseButtonPressed_Impl(EMouseCode Button)
{
	GLFWwindow* Window = (GLFWwindow*) FApplication::Get()->GetWindow()->GetNativeWindowHandle();
	int Status = glfwGetMouseButton(Window, Button);
	return Status == GLFW_PRESS;
}

glm::vec2 FGenericInput::GetMousePosition_Impl()
{
	GLFWwindow* Window = (GLFWwindow*)FApplication::Get()->GetWindow()->GetNativeWindowHandle();
	double X, Y;

	glfwGetCursorPos(Window, &X, &Y);
	return { (float) X, (float) Y };
}
