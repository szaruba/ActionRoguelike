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

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION()
	void HandlePawnKilled(APawn* KilledPawn, APawn* InstigatorPawn);
protected:

	UPROPERTY(EditAnywhere)
	float BotSpawnRate;
	UPROPERTY(EditAnywhere)
	UCurveFloat* MaxBotCurve;

	UPROPERTY(EditAnywhere)
	UEnvQuery* EnvQuery_FindBotSpawn;

	UPROPERTY(EditAnywhere)
	UEnvQuery* EnvQuery_FindCoinSpawn;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASAICharacter> BotCharacterClass;

	UPROPERTY(EditAnywhere)
	float CoinSpawnRate;
	
	virtual void StartPlay() override;
	void SpawnCoin();

	UFUNCTION()
	void RunSpawnBotQuery();
	void RespawnPlayer(APlayerController* PlayerController);
	void DisposeCorpse(APawn* KilledPawn);

	void TestFunction1();
	bool TestFunction2();
	void TestFunction3(float Value);
	bool TestFunction4(float Value);
	

private:
	UFUNCTION()
	void HandleSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void HandleSpawnCoinQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
};
