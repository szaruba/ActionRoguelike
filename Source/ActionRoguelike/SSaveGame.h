// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SSaveGame.generated.h"

USTRUCT()
struct FSaveDataActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;
	
	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT()
struct FSaveDataPlayer
{
	GENERATED_BODY()

	UPROPERTY()
	FString Id;

	UPROPERTY()
	int32 Credits;
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	TArray<FSaveDataActor> SavedActors;

	UPROPERTY()
	TArray<FSaveDataPlayer> SavedPlayers;

	UPROPERTY()
	int32 Credits;
};
