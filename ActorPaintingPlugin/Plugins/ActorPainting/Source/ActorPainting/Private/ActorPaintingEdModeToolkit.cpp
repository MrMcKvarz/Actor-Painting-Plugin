// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ActorPaintingPrivatePCH.h"
#include "ActorPaintingEdMode.h"
#include "ActorPaintingEdModeToolkit.h"
#include "SNumericEntryBox.h"
#include "PackageTools.h"
#include "ISourceControlModule.h"
#include "SColorPicker.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Selection.h"
#include "Engine/StaticMesh.h"

#define LOCTEXT_NAMESPACE "FActorPaintingEdModeToolkit"


FActorPaintingEdModeToolkit::FActorPaintingEdModeToolkit()
{
	PaintColor = (FLinearColor(100, 100, 100));
}

void FActorPaintingEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	FMargin StandardPadding(0.0f, 4.0f, 0.0f, 4.0f);

	float MinBrushSliderRadius = 1, MaxBrushSliderRadius = 256;

	float MinBrushRadius = 1, MaxBrushRadius = 256;

	struct Locals
	{

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}
	};

	const float Factor = 256.0f;

	ChildSlot
	[
			SAssignNew(ToolkitWidget, SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(6.0f, 0.0f)
			.FillHeight(1.f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 8.0f, 0.0f, 8.0f)
				.HAlign(HAlign_Left)
				[
					SNew(STextBlock)
					.AutoWrapText(true)
					.Text(LOCTEXT("PaintActorsLabel", "Paint Actors"))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(StandardPadding)
				[
					SNew(SSeparator)
					.Orientation(Orient_Horizontal)
				]
				+ SVerticalBox::Slot() // Vertex instances panel slot
				.AutoHeight()
				.Padding(StandardPadding)
				[
					#pragma region VertexInstancesPanel
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.AutoHeight()
					.Padding(StandardPadding)
					[
						#pragma region HorizontalBoxForInstanceVertexLabels
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(2.0f, 0.0f)
						.AutoWidth()
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.AutoWrapText(true)
							.Text(LOCTEXT("VertexColorsLabel", "Instance vertex colors"))
						]
						+ SHorizontalBox::Slot()
						.Padding(2.0f, 0.0f)
						.HAlign(HAlign_Right)
						[
							SNew(STextBlock)
							.AutoWrapText(true)
							.Text(LOCTEXT("IDKWhatLabel", "None")) //TODO Check what this label actually do
						]
						#pragma endregion
					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.AutoHeight()
					.Padding(StandardPadding)
					[
						#pragma region HorizontalBoxForButton
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.FillWidth(1)
								[
									SNew(SWrapBox)
									.UseAllottedWidth(true)
									+ SWrapBox::Slot()
									.Padding(2.0f, 0.0f)
									[
										SNew(SButton)
										.Text(LOCTEXT("CopyButtonLabel", "Copy"))
										.HAlign(HAlign_Right)
										.VAlign(VAlign_Center)
									]
									+ SWrapBox::Slot()
									.Padding(2.0f, 0.0f)
									[
										SNew(SButton)
										.Text(LOCTEXT("PasteButtonLabel", "Paste"))
										.HAlign(HAlign_Right)
										.VAlign(VAlign_Center)
									]
									+ SWrapBox::Slot()
									.Padding(2.0f, 0.0f)
									[
										SNew(SButton)
										.Text(LOCTEXT("RemoveButtonLabel", "Remove"))
										.HAlign(HAlign_Right)
										.VAlign(VAlign_Center)
									]
									+ SWrapBox::Slot()
									.Padding(2.0f, 0.0f)
									[
										SNew(SButton)
										.Text(LOCTEXT("FixButtonLabel", "Fix"))
										.HAlign(HAlign_Right)
										.VAlign(VAlign_Center)
									]
								]
								#pragma endregion
					]
					#pragma endregion
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(StandardPadding)
				[
					SNew(SSeparator)
					.Orientation(Orient_Horizontal)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(StandardPadding)
				[
					#pragma region ColorModePanel
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(StandardPadding)
					.HAlign(HAlign_Fill)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(StandardPadding)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.AutoWrapText(true)
							.Text(LOCTEXT("ModeLabel", "Mode"))
						]
						+ SHorizontalBox::Slot()
						.Padding(StandardPadding)
						.HAlign(HAlign_Right)
						[
							SNew(STextBlock)
							.AutoWrapText(true)
							.Text(LOCTEXT("SholdReallyBeHereLabel", "Color  Blend Weights")) // TODO Check if is needed
						]
					]
					#pragma endregion
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(StandardPadding)
				[
					SNew(SSeparator)
					.Orientation(Orient_Horizontal)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(StandardPadding)
				[
					#pragma region PaintColor
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(StandardPadding)
					.HAlign(HAlign_Fill)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(StandardPadding)
						.HAlign(HAlign_Left)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(StandardPadding)
							[
								SNew(STextBlock)
								.AutoWrapText(true)
								.Text(LOCTEXT("PaintLabel", "Paint Color"))
							]
						]
						+ SHorizontalBox::Slot()
						.Padding(6.0f, 0.0f, 0.0f, 0.0f)
						.HAlign(HAlign_Fill)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.Padding(StandardPadding)
							.HAlign(HAlign_Fill)
							[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.FillWidth(1)
							.Padding(StandardPadding)
							[
								SNew(SColorBlock)
								.ShowBackgroundForAlpha(true)
								.OnMouseButtonDown(this, &FActorPaintingEdModeToolkit::PaintColorBlock_OnMouseButtonDown)
								.Color(this, &FActorPaintingEdModeToolkit::SetCBColor)
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(StandardPadding)
				.HAlign(HAlign_Fill)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(StandardPadding)
						.HAlign(HAlign_Left)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(StandardPadding)
							[
							SNew(STextBlock)
							.AutoWrapText(true)
							.Text(LOCTEXT("ChannelsLabel", "Channels"))
							]
						]
						+ SHorizontalBox::Slot()
						.Padding(StandardPadding)
						.HAlign(HAlign_Right)
						[
							SNew(SVerticalBox) // TODO Fix alignment
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(StandardPadding)
							[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(2.0f, 0.0f)
							[
								SNew(SCheckBox)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("RedChannelLabel", "Red"))
								]
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(2.0f, 0.0f)
							[
								SNew(SCheckBox)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("GreenChannelLabel", "Green"))
								]
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(2.0f, 0.0f)
							[
								SNew(SCheckBox)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("BlueChannelLabel", "Blue"))
								]
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(2.0f, 0.0f)
							[
								SNew(SCheckBox)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("AplhaChannelLabel", "Alpha"))
								]
							]
						]
					]
				]
					#pragma endregion
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(StandardPadding)
				[
					SNew(SSeparator)
					.Orientation(Orient_Horizontal)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(StandardPadding)
				[
					#pragma region PaintToolSettings
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(StandardPadding)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.AutoWrapText(true)
							.Text(LOCTEXT("RadiusLabel","Radius"))
						]
						+ SHorizontalBox::Slot()
						.FillWidth(2.0f)
						.Padding(StandardPadding)	
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Center)
						[
							SNew(SNumericEntryBox<float>)
							.AllowSpin(true)
							.MinSliderValue(MinBrushSliderRadius)
							.MaxSliderValue(MaxBrushSliderRadius)
							.MinValue(MinBrushRadius)
							.MaxValue(MaxBrushRadius)
							.Value(0)
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(StandardPadding)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.AutoWrapText(true)
							.Text(LOCTEXT("StrengthLabel", "Strength"))
						]
						+ SHorizontalBox::Slot()
						.FillWidth(2.0f)
						.Padding(StandardPadding)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Center)
						[
							SNew(SNumericEntryBox<float>) //TODO Loose magic numbers
							.AllowSpin(true)
							.MinSliderValue(0.f)
							.MaxSliderValue(1.f)
							.MinValue(0.f)
							.MaxValue(1.f)
							.Value(0.f)
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(StandardPadding)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.AutoWrapText(true)
							.Text(LOCTEXT("FalloffLabel", "Falloff"))
						]
						+ SHorizontalBox::Slot()
						.FillWidth(2.0f)
						.Padding(StandardPadding)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Center)
						[
							SNew(SNumericEntryBox<float>) 
							.AllowSpin(true)
							.MinSliderValue(0.f)
							.MaxSliderValue(1.f)
							.MinValue(0.f)
							.MaxValue(1.f)
							.Value(0.f)
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(StandardPadding)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.AutoWrapText(true)
						.Text(LOCTEXT("BrushFlowLabel", "Enable brush flow"))
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						[
							SNew(SCheckBox)
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(StandardPadding)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.AutoWrapText(true)
							.Text(LOCTEXT("FlowLabel", "Flow"))
						]
						+ SHorizontalBox::Slot()
						.FillWidth(2.0f)
						.Padding(StandardPadding)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Center)
						[
							SNew(SNumericEntryBox<float>)
							.AllowSpin(true)
							.MinSliderValue(0.f)
							.MaxSliderValue(1.f)
							.MinValue(0.f)
							.MaxValue(1.f)
							.Value(0.f)
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(StandardPadding)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.AutoWrapText(true)
						.Text(LOCTEXT("IngoreBlackFacingLabel", "Ignore black-facing"))
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						[
							SNew(SCheckBox)
						]
					]
					#pragma endregion
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(StandardPadding)
				[
					#pragma region ViewPanel
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(2.0f, 0.0f)
					.FillWidth(1)
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ViewLabel", "View"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(0.0f, 0.0f, 2.0f, 0.0f)
					.HAlign(HAlign_Right)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("MeshPaint_VertexColorViewLabel", "Off  RGB  R  G  B  A"))
					]
					#pragma endregion
				]				
			]
		];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FActorPaintingEdModeToolkit::GetToolkitFName() const
{
	return FName("ActorPaintingEdMode");
}

FText FActorPaintingEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("ActorPaintingEdModeToolkit", "DisplayName", "ActorPaintingEdMode Tool");
}

class FEdMode* FActorPaintingEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FActorPaintingEdMode::EM_ActorPaintingEdModeId);
}

FReply FActorPaintingEdModeToolkit::PaintColorBlock_OnMouseButtonDown(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
	{
		return FReply::Unhandled();
	}

	CreateColorPickerWindow();

	/*const UProperty* Property = PropertyEditor->GetProperty();
	check(Property);

	bool bRefreshOnlyOnOk = Property->GetOwnerClass() && Property->GetOwnerClass()->IsChildOf(UMaterialExpressionConstant3Vector::StaticClass());
	//@todo Slate Property window: This should probably be controlled via metadata and then the actual alpha channel property hidden if its not used.
	const bool bUseAlpha = !(Property->GetOwnerClass() && (Property->GetOwnerClass()->IsChildOf(ULightComponent::StaticClass()) || bRefreshOnlyOnOk));*/

	return FReply::Handled();
}

void FActorPaintingEdModeToolkit::SetColor(FLinearColor NewColor)
{
	PaintColor = NewColor;
	UE_LOG(LogTemp, Warning, TEXT("Original Colors %s"), *PaintColor.ToString());
}

void FActorPaintingEdModeToolkit::OnColorPickerCancelled(FLinearColor OriginalColor)
{

}

void FActorPaintingEdModeToolkit::CreateColorPickerWindow()
{
	//TODO set the property "Value" in SHV to proper value
	FColorPickerArgs PickerArgs;
	PickerArgs.bUseAlpha = true;
	PickerArgs.ParentWidget = ParentWidget;
	PickerArgs.DisplayGamma = TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(GEngine, &UEngine::GetDisplayGamma));
	PickerArgs.bOnlyRefreshOnOk = true;
	PickerArgs.bOnlyRefreshOnMouseUp = true;
	PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &FActorPaintingEdModeToolkit::SetColor);
	PickerArgs.OnColorPickerCancelled = FOnColorPickerCancelled::CreateSP(this, &FActorPaintingEdModeToolkit::OnColorPickerCancelled);

	PickerArgs.InitialColorOverride = FLinearColor(0,0,1,1);

	OpenColorPicker(PickerArgs);

}

FLinearColor FActorPaintingEdModeToolkit::SetCBColor() const
{
	return PaintColor;
}




#undef LOCTEXT_NAMESPACE
