// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCoinPickup.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SaveGame.h"
#include "SGameModeBase.generated.h"


class USSaveGame;
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

	UFUNCTION(BlueprintCallable)
	void WriteSaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadSaveGame();
	
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASCoinPickup> SpawnedCoinClass;
	
	virtual void StartPlay() override;
	void SpawnCoin();

	UFUNCTION()
	void RunSpawnBotQuery();
	void RespawnPlayer(APlayerController* PlayerController);
	void DisposeCorpse(APawn* KilledPawn);


	virtual void Tick(float DeltaSeconds) override;
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

private:
	UFUNCTION()
	void HandleSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void HandleSpawnCoinQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	FString SlotName;

	UPROPERTY()
	USSaveGame* CurrentSaveGame;
};
