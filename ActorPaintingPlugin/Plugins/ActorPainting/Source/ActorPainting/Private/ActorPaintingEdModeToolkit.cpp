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
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

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

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Left)
		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Left)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.AutoWrapText(true)
			.Text(LOCTEXT("VertexColorsLabel", "Instance vertex colors"))
			]
			]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("CopyButtonLabel", "Copy"), FVector(0, 0, Factor))

						]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("PasteButtonLabel", "Paste"), FVector(0, -Factor, 0))
						]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("RemoveButtonLabel", "Remove"), FVector(0, Factor, 0))
						]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("FixButtonLabel", "Fix"), FVector(0, 0, -Factor))
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
