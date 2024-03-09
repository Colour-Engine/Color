#include "ColorPCH.h"
#include "MessageDialog.h"

#include "Platform/Windows/WindowsMessageDialog.h"
#include "Platform/Linux/LinuxMessageDialog.h"

TScope<FMessageDialog> FMessageDialog::New(const FString& Caption, const FString& Content, EDialogControls Controls, EDialogIcon Icon)
{
	return MakeScope<ConcatWithPlatformNameDef(MessageDialog)>(Caption, Content, Controls, Icon);
}
