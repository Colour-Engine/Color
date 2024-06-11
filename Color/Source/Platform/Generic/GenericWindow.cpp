#include "ColorPCH.h"
#include "GenericWindow.h"

#include "Renderer/RendererAPI.h"

#include <GLFW/glfw3.h>

static uint16 WindowCount = 0;

static void GLFWErrorCallback(int ErrorCode, const char* Description)
{
	CL_CORE_ERROR("GLFW Error (%d): %s", ErrorCode, Description);
}

FGenericWindow::FGenericWindow(const FWindowProps& Props)
	: Data
	  {
	      Props.Title,
	      Props.Width,
	      Props.Height,
	      Props.bFullscreen,
	      Props.bResizable,
	      Props.bVSync,

		  OnClose,
		  OnResize,
		  OnMove,
		  OnFocusChange,
		  OnKeyPress,
		  OnKeyRelease,
		  OnKeyType,
		  OnMouseButtonPress,
		  OnMouseButtonRelease,
		  OnMouseMove,
		  OnMouseScroll
	  }
{
}

FGenericWindow::~FGenericWindow()
{
	if (IsOpen())
	{
		Destroy();
	}
}

bool FGenericWindow::Init()
{
	if (IsOpen())
	{
		CL_CORE_WARN("Window is already open, discarding the Init request.");
		return false;
	}

	CL_CORE_INFO("Creating window \"%s\" (%dx%d)", *Data.Title, Data.Width, Data.Height);

	if (WindowCount == 0)
	{
		if (!glfwInit())
		{
			CL_CORE_ERROR("Failed to initialize GLFW!");
			return false;
		}

		glfwSetErrorCallback(GLFWErrorCallback);
	}

	glfwWindowHint(GLFW_RESIZABLE, Data.bResizable ? GLFW_TRUE : GLFW_FALSE);
	if (FRendererAPI::GetAPI() == ERendererAPI::OpenGL)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#ifndef CL_SHIPPING
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	}

	GLFWmonitor* Monitor = Data.bFullscreen ? glfwGetPrimaryMonitor() : nullptr;
	Handle = glfwCreateWindow
	(
		Data.Width,
		Data.Height,
		Data.Title.Get(),
		Monitor,
		nullptr
	);

	if (!Handle)
	{
		glfwTerminate();
		CL_CORE_ERROR("Failed to create the GLFW window handle!");
		return false;
	}

	Context = FGraphicsContext::New(Handle);
	if (!Context->Init())
	{
		glfwDestroyWindow(Handle);
		glfwTerminate();

		return false;
	}

	glfwSetWindowUserPointer(Handle, &Data);
	SetVSync(Data.bVSync);

	glfwSetWindowCloseCallback(Handle, [](GLFWwindow* Window)
	{
		FWindowData& Data = *(FWindowData*) glfwGetWindowUserPointer(Window);
		Data.OnClose.Broadcast();
	});

	glfwSetWindowSizeCallback(Handle, [](GLFWwindow* Window, int Width, int Height)
	{
		FWindowData& Data = *(FWindowData*) glfwGetWindowUserPointer(Window);
		
		Data.Width = Width;
		Data.Height = Height;

		Data.OnResize.Broadcast(Width, Height);
	});

	glfwSetWindowPosCallback(Handle, [](GLFWwindow* Window, int PosX, int PosY)
	{
		FWindowData& Data = *(FWindowData*) glfwGetWindowUserPointer(Window);
		Data.OnMove.Broadcast(PosX, PosY);
	});

	glfwSetWindowFocusCallback(Handle, [](GLFWwindow* Window, int bFocus)
	{
		FWindowData& Data = *(FWindowData*) glfwGetWindowUserPointer(Window);
		Data.OnFocusChange.Broadcast(bFocus);
	});

	glfwSetKeyCallback(Handle, [](GLFWwindow* Window, int KeyCode, int ScanCode, int Action, int Mods)
	{
		FWindowData& Data = *(FWindowData*) glfwGetWindowUserPointer(Window);

		switch (Action)
		{
		case GLFW_PRESS:
		{
			Data.OnKeyPress.Broadcast(KeyCode, false);
			break;
		}
		case GLFW_REPEAT:
		{
			Data.OnKeyPress.Broadcast(KeyCode, true);
			break;
		}
		case GLFW_RELEASE:
		{
			Data.OnKeyRelease.Broadcast(KeyCode);
			break;
		}
		}
	});

	glfwSetCharCallback(Handle, [](GLFWwindow* Window, unsigned int KeyCode)
	{
		FWindowData& Data = *(FWindowData*) glfwGetWindowUserPointer(Window);
		Data.OnKeyType.Broadcast(KeyCode);
	});

	glfwSetMouseButtonCallback(Handle, [](GLFWwindow* Window, int Button, int Action, int Mods)
	{
		FWindowData& Data = *(FWindowData*) glfwGetWindowUserPointer(Window);
		
		switch (Action)
		{
		case GLFW_PRESS:
		{
			Data.OnMouseButtonPress.Broadcast(Button);
			break;
		}
		// Unlike Keys, MouseButtons don't repeat so GLFW_REPEAT is absent here.
		case GLFW_RELEASE:
		{
			Data.OnMouseButtonRelease.Broadcast(Button);
			break;
		}
		}
	});

	glfwSetCursorPosCallback(Handle, [](GLFWwindow* Window, double PosX, double PosY)
	{
		FWindowData& Data = *(FWindowData*) glfwGetWindowUserPointer(Window);
		Data.OnMouseMove.Broadcast((float) PosX, (float) PosY);
	});

	glfwSetScrollCallback(Handle, [](GLFWwindow* Window, double OffsetX, double OffsetY)
	{
		FWindowData& Data = *(FWindowData*) glfwGetWindowUserPointer(Window);
		Data.OnMouseScroll.Broadcast((float) OffsetX, (float) OffsetY);
	});

	WindowCount++;
	return true;
}

bool FGenericWindow::IsOpen() const
{
	return Handle && !glfwWindowShouldClose(Handle);
}

void FGenericWindow::Update()
{
	glfwPollEvents();
	Context->SwapBuffers();
}

bool FGenericWindow::Destroy()
{
	if (Handle)
	{
		glfwDestroyWindow(Handle);
		Handle = nullptr;

		if (--WindowCount == 0)
		{
			CL_CORE_WARN("Last remaining window got a destruction request, therefore, GLFW will be terminated.");
			glfwTerminate();
		}

		return true;
	}

	return false;
}

void FGenericWindow::SetTitle(const FString& NewTitle)
{
	Data.Title = NewTitle;
	glfwSetWindowTitle(Handle, *NewTitle);
}

void FGenericWindow::SetSize(uint32 NewWidth, uint32 NewHeight)
{
	glfwSetWindowSize(Handle, NewWidth, NewHeight);
}

void FGenericWindow::SetWidth(uint32 NewWidth)
{
	SetSize(NewWidth, Data.Height);
}

void FGenericWindow::SetHeight(uint32 NewHeight)
{
	SetSize(Data.Width, NewHeight);
}

void FGenericWindow::SetPos(const glm::ivec2& NewPosition)
{
	glfwSetWindowPos(Handle, NewPosition.x, NewPosition.y);
}

void FGenericWindow::SetVSync(bool bNewVSync)
{
	Data.bVSync = bNewVSync;
	glfwSwapInterval(bNewVSync ? 1 : 0);
}

void FGenericWindow::Focus()
{
	glfwFocusWindow(Handle);
}

glm::ivec2 FGenericWindow::GetPos() const
{
	glm::ivec2 Result;
	glfwGetWindowPos(Handle, &Result.x, &Result.y);

	return Result;
}

uint32 FGenericWindow::GetPosX() const
{
	return GetPos().x;
}

uint32 FGenericWindow::GetPosY() const
{
	return GetPos().y;
}

bool FGenericWindow::HasFocus() const
{
	return glfwGetWindowAttrib(Handle, GLFW_FOCUSED);
}
