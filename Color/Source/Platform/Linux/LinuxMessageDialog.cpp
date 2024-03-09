#include "ColorPCH.h"
#include "LinuxMessageDialog.h"

FLinuxMessageDialog::FLinuxMessageDialog(const FString& InCaption, const FString& InContent, EDialogControls InControls, EDialogIcon InIcon)
	: Caption(InCaption), Content(InContent), Controls(InControls), Icon(InIcon)
{
}

FLinuxMessageDialog::~FLinuxMessageDialog()
{
}

EDialogResult FLinuxMessageDialog::CreateAndAwait()
{
	return EDialogResult::CreationFailure;
}

bool FLinuxMessageDialog::SupportsControls(EDialogControls Controls) const
{
	// Linux supports NONE of the controls.
	return false;
}

bool FLinuxMessageDialog::SupportsIcon(EDialogIcon Icon) const
{
	// Linux supports NONE of the icons.
	return false;
}

void FLinuxMessageDialog::SetCaption(const FString& NewCaption)
{
	Caption = NewCaption;
}

void FLinuxMessageDialog::SetContent(const FString& NewContent)
{
	Content = NewContent;
}

void FLinuxMessageDialog::SetControls(EDialogControls NewControls)
{
	Controls = NewControls;
}

void FLinuxMessageDialog::SetIcon(EDialogIcon NewIcon)
{
	Icon = NewIcon;
}
