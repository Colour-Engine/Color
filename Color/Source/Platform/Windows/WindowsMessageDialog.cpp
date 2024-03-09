#include "ColorPCH.h"
#include "WindowsMessageDialog.h"

FWindowsMessageDialog::FWindowsMessageDialog(const FString& InCaption, const FString& InContent, EDialogControls InControls, EDialogIcon InIcon)
	: Caption(InCaption), Content(InContent), Controls(InControls), Icon(InIcon)
{
}

FWindowsMessageDialog::~FWindowsMessageDialog()
{
}

EDialogResult FWindowsMessageDialog::CreateAndAwait()
{
	return (EDialogResult) MessageBoxA(NULL, *Content, *Caption, (UINT)((long) Controls | (long) Icon));
}

bool FWindowsMessageDialog::SupportsControls(EDialogControls Controls) const
{
	// Windows supports all of the controls.
	return true;
}

bool FWindowsMessageDialog::SupportsIcon(EDialogIcon Icon) const
{
	// Windows supports all of the icons.
	return true;
}

void FWindowsMessageDialog::SetCaption(const FString& NewCaption)
{
	Caption = NewCaption;
}

void FWindowsMessageDialog::SetContent(const FString& NewContent)
{
	Content = NewContent;
}

void FWindowsMessageDialog::SetControls(EDialogControls NewControls)
{
	Controls = NewControls;
}

void FWindowsMessageDialog::SetIcon(EDialogIcon NewIcon)
{
	Icon = NewIcon;
}
