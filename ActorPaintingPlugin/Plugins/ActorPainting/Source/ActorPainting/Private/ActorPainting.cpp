// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ActorPaintingPrivatePCH.h"
#include "ActorPaintingEdMode.h"

#define LOCTEXT_NAMESPACE "FActorPaintingModule"

void FActorPaintingModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FActorPaintingEdMode>(FActorPaintingEdMode::EM_ActorPaintingEdModeId, LOCTEXT("ActorPaintingEdModeName", "ActorPaintingEdMode"), FSlateIcon(), true);
}

void FActorPaintingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FActorPaintingEdMode::EM_ActorPaintingEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FActorPaintingModule, ActorPainting)