// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ASGameModeBase::ASGameModeBase()
{
	BotSpawnRate = 5.f;
}

void ASGameModeBase::KillAll()
{
	for (ASAICharacter* AICharacter : TActorRange<ASAICharacter>(GetWorld()))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AICharacter);
		AttributeComponent->ApplyHealthChange(this, -AttributeComponent->GetHealthMax()); // @Fixme: set instigator actor to issuing player of KillAll command
	}
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASGameModeBase::RunSpawnBotQuery, BotSpawnRate, true);
}

void ASGameModeBase::RunSpawnBotQuery()
{
	int BotCount = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		++BotCount;
	}

	float BotCountMax = MaxBotCurve ? MaxBotCurve->GetFloatValue(GetWorld()->GetTimeSeconds()) : 5;

	if (BotCount < BotCountMax)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, EnvQuery_FindBotSpawn, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::HandleSpawnBotQueryFinished);
	}
}

void ASGameModeBase::HandleSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	TArray<FVector> BotSpawnLocations = QueryInstance->GetResultsAsLocations();
	if (QueryStatus == EEnvQueryStatus::Type::Success && BotSpawnLocations.IsValidIndex(0))
	{
		FVector SpawnLocation = BotSpawnLocations[0];
		GetWorld()->SpawnActor(BotCharacterClass, &SpawnLocation);
	}

}
