// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameModeBase();
	
protected:

	UPROPERTY(EditAnywhere)
	float BotSpawnRate;
	UPROPERTY(EditAnywhere)
	UCurveFloat* MaxBotCurve;

	UPROPERTY(EditAnywhere)
	UEnvQuery* EnvQuery_FindBotSpawn;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASAICharacter> BotCharacterClass;
	
	virtual void StartPlay() override;

	UFUNCTION()
	void RunSpawnBotQuery();

private:
	UFUNCTION()
	void HandleSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
};
