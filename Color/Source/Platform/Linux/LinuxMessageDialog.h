#pragma once

#include "Misc/MessageDialog.h"

class FLinuxMessageDialog : public FMessageDialog
{
public:
	FLinuxMessageDialog(const FString& InCaption, const FString& InContent, EDialogControls InControls, EDialogIcon InIcon);
	~FLinuxMessageDialog() override;

	virtual EDialogResult CreateAndAwait() override;
	virtual bool SupportsControls(EDialogControls Controls) const override;
	virtual bool SupportsIcon(EDialogIcon Icon) const override;

	virtual void SetCaption(const FString& NewCaption) override;
	virtual void SetContent(const FString& NewContent) override;
	virtual void SetControls(EDialogControls NewControls) override;
	virtual void SetIcon(EDialogIcon NewIcon) override;

	virtual const FString& GetCaption() const override { return Caption; }
	virtual const FString& GetContent() const override { return Content; }
	virtual EDialogControls GetControls() const override { return Controls; }
	virtual EDialogIcon GetIcon() const override { return Icon; }
private:
	FString Caption, Content;
	EDialogControls Controls;
	EDialogIcon Icon;
};
