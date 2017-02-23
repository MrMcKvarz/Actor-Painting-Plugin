// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ActorPaintingPrivatePCH.h"
#include "ActorPaintingEdMode.h"
#include "ActorPaintingEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"

const FEditorModeID FActorPaintingEdMode::EM_ActorPaintingEdModeId = TEXT("EM_ActorPaintingEdMode");

FActorPaintingEdMode::FActorPaintingEdMode()
{

}

FActorPaintingEdMode::~FActorPaintingEdMode()
{

}

void FActorPaintingEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FActorPaintingEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FActorPaintingEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FActorPaintingEdMode::UsesToolkits() const
{
	return true;
}




