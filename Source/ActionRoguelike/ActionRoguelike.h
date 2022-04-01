// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
	if(!ensure(WorldContext))
	{
		return;
	}
	
	UWorld* World = WorldContext->GetWorld();
	if(!ensure(World))
	{
		return;
	}
	
	FString NetPrefix = World->IsServer() ? "[SERVER] " : "[CLIENT] ";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, Duration, Color, NetPrefix + Msg);
	}
}