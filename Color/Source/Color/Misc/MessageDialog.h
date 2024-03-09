#pragma once

#include "Containers/String.h"
#include "Templates/Scope.h"

enum class EDialogControls : long
{
	AbortRetryIgnore  = 0x00000002L,
	CancelTryContinue = 0x00000006L,
	Help              = 0x00004000L,
	Ok                = 0x00000000L,
	OkCancel          = 0x00000001L,
	RetryCancel       = 0x00000005L,
	YesNo             = 0x00000004L,
	YesNoCancel       = 0x00000003L
};

enum class EDialogIcon : long
{
	Question    = 0x00000020L,
	Information = 0x00000040L,
	Warning     = 0x00000030L,
	Error       = 0x00000010L
};

enum class EDialogResult : int32
{
	// Exceptional case where the dialog itself wasn't created at all.
	CreationFailure = -1,

	Ok       = 1,
	Cancel   = 2,
	Abort    = 3,
	Retry    = 4,
	Ignore   = 5,
	Yes      = 6,
	No       = 7,
	TryAgain = 10,
	Continue = 11
};

class FMessageDialog
{
public:
	virtual ~FMessageDialog() = default;

	virtual EDialogResult CreateAndAwait() = 0;
	virtual bool SupportsControls(EDialogControls Controls) const = 0;
	virtual bool SupportsIcon(EDialogIcon Icon) const = 0;

	virtual void SetCaption(const FString& NewCaption) = 0;
	virtual void SetContent(const FString& NewContent) = 0;
	virtual void SetControls(EDialogControls NewControls) = 0;
	virtual void SetIcon(EDialogIcon NewIcon) = 0;

	virtual const FString& GetCaption() const = 0;
	virtual const FString& GetContent() const = 0;
	virtual EDialogControls GetControls() const = 0;
	virtual EDialogIcon GetIcon() const = 0;

	static TScope<FMessageDialog> New(const FString& Caption, const FString& Content, EDialogControls Controls = EDialogControls::Ok, EDialogIcon Icon = EDialogIcon::Information);
};
