// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Editor/UnrealEd/Public/Toolkits/BaseToolkit.h"


class FActorPaintingEdModeToolkit : public FModeToolkit
{
public:

	FActorPaintingEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

private:

	TSharedPtr<SWidget> ToolkitWidget;
	FSimpleSlot ChildSlot;

	FReply PaintColorBlock_OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	void SetColor(FLinearColor NewColor);

	void OnColorPickerCancelled(FLinearColor OriginalColor);

};

