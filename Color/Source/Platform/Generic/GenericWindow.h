#pragma once

#include "Core/Window.h"
#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

class FGenericWindow : public FWindow
{
public:
	FGenericWindow(const FWindowProps& Props = {});
	~FGenericWindow() override;

	virtual bool Init() override;
	virtual bool IsOpen() const override;
	virtual void Update() override;
	virtual bool Destroy() override;

	virtual void SetTitle(const FString& NewTitle) override;
	virtual void SetSize(uint32 NewWidth, uint32 NewHeight) override;
	virtual void SetWidth(uint32 NewWidth) override;
	virtual void SetHeight(uint32 NewHeight) override;
	virtual void SetPos(const glm::ivec2& NewPosition) override;
	virtual void SetVSync(bool bNewVSync) override;
	virtual void Focus() override;

	virtual const FString& GetTitle() const override { return Data.Title; }
	virtual glm::ivec2 GetSize() const override { return { Data.Width, Data.Height }; }
	virtual uint32 GetWidth() const override { return Data.Width; }
	virtual uint32 GetHeight() const override { return Data.Height; }
	virtual glm::ivec2 GetPos() const override;
	virtual uint32 GetPosX() const override;
	virtual uint32 GetPosY() const override;
	virtual bool IsFullscreen() const override { return Data.bFullscreen; }
	virtual bool IsVSync() const override { return Data.bVSync; }
	virtual bool HasFocus() const override;
	virtual void* GetNativeWindowHandle() const override { return Handle; }
private:
	GLFWwindow* Handle = nullptr;

	struct FWindowData
	{
		FString Title;
		uint32  Width, Height;
		bool    bFullscreen, bResizable, bVSync;

		FWindowCloseEvent&        OnClose;
		FWindowResizeEvent&       OnResize;
		FWindowMoveEvent&         OnMove;
		FWindowFocusChangeEvent&  OnFocusChange;
		FKeyPressEvent&           OnKeyPress;
		FKeyReleaseEvent&         OnKeyRelease;
		FKeyTypeEvent&            OnKeyType;
		FMouseButtonPressEvent&   OnMouseButtonPress;
		FMouseButtonReleaseEvent& OnMouseButtonRelease;
		FMouseMoveEvent&          OnMouseMove;
		FMouseScrollEvent&        OnMouseScroll;
	} Data;

	TScope<FGraphicsContext> Context;
};
