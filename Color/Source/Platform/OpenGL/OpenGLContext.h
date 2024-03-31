#pragma once

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

class FOpenGLContext : public FGraphicsContext
{
public:
	FOpenGLContext(GLFWwindow* InNativeWindowHandle);
	
	virtual bool Init() override;
	virtual void SwapBuffers() override;
private:
	GLFWwindow* NativeWindowHandle = nullptr;
};
