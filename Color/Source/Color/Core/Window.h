#pragma once

#include "Core/Base.h"

#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"

#include "Utils/Event.h"

#include <glm/glm.hpp>

DECLARE_EVENT(FWindowCloseEvent);
DECLARE_EVENT(FWindowResizeEvent, uint32 /*Width*/, uint32 /*Height*/);
DECLARE_EVENT(FWindowMoveEvent, uint32 /*PosX*/, uint32 /*PosY*/);
DECLARE_EVENT(FWindowFocusChangeEvent, bool /*bNewFocus*/);
DECLARE_EVENT(FKeyPressEvent, EKeyCode /*KeyCode*/, bool /*bIsRepeat*/);
DECLARE_EVENT(FKeyReleaseEvent, EKeyCode /*KeyCode*/);
DECLARE_EVENT(FKeyTypeEvent, EKeyCode /*KeyCode*/);
DECLARE_EVENT(FMouseButtonPressEvent, EMouseCode /*Button*/);
DECLARE_EVENT(FMouseButtonReleaseEvent, EMouseCode /*Button*/);
DECLARE_EVENT(FMouseMoveEvent, float /*PosX*/, float /*PosY*/);
DECLARE_EVENT(FMouseScrollEvent, float /*OffsetX*/, float /*OffsetY*/);

struct FWindowProps
{
	FString Title;
	uint32  Width, Height;
	bool    bFullscreen, bResizable, bVSync;

	FWindowProps(const FString& Title = "Color Window",
		uint32         Width = 1600,
		uint32         Height = 900,
		bool           bFullscreen = false,
		bool           bResizable = true,
		bool           bVSync = true)
		: Title(Title), Width(Width), Height(Height), bFullscreen(bFullscreen), bResizable(bResizable), bVSync(bVSync)
	{
	}
};

class FWindow
{
public:
	FWindowCloseEvent        OnClose;
	FWindowResizeEvent       OnResize;
	FWindowMoveEvent         OnMove;
	FWindowFocusChangeEvent  OnFocusChange;
	FKeyPressEvent           OnKeyPress;
	FKeyReleaseEvent         OnKeyRelease;
	FKeyTypeEvent            OnKeyType;
	FMouseButtonPressEvent   OnMouseButtonPress;
	FMouseButtonReleaseEvent OnMouseButtonRelease;
	FMouseMoveEvent          OnMouseMove;
	FMouseScrollEvent        OnMouseScroll;
public:
	virtual ~FWindow() = default;

	virtual bool Init() = 0;
	virtual bool IsOpen() const = 0;
	virtual void Update() = 0;
	virtual bool Destroy() = 0;

	virtual void SetTitle(const FString& NewTitle) = 0;
	virtual void SetSize(uint32 NewWidth, uint32 NewHeight) = 0;
	virtual void SetWidth(uint32 NewWidth) = 0;
	virtual void SetHeight(uint32 NewHeight) = 0;
	virtual void SetPos(const glm::ivec2& NewPosition) = 0;
	virtual void SetVSync(bool bNewVSync) = 0;
	virtual void Focus() = 0;

	virtual const FString& GetTitle() const = 0;
	virtual glm::ivec2 GetSize() const = 0;
	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;
	virtual glm::ivec2 GetPos() const = 0;
	virtual uint32 GetPosX() const = 0;
	virtual uint32 GetPosY() const = 0;
	virtual bool IsFullscreen() const = 0;
	virtual bool IsVSync() const = 0;
	virtual bool HasFocus() const = 0;
	virtual void* GetNativeWindowHandle() const = 0;

	static TScope<FWindow> New(const FWindowProps& Props = {});
};
