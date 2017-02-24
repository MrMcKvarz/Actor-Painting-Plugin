// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ActorPaintingPrivatePCH.h"
#include "ActorPaintingEdMode.h"
#include "ActorPaintingEdModeToolkit.h"

#define LOCTEXT_NAMESPACE "FActorPaintingEdModeToolkit"

FActorPaintingEdModeToolkit::FActorPaintingEdModeToolkit()
{
}

void FActorPaintingEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	FMargin StandardPadding(0.0f, 4.0f, 0.0f, 4.0f);
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

#undef LOCTEXT_NAMESPACE
