// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable/Disable bot spawning"), ECVF_Cheat);

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
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Display, TEXT("Bot is not spawned because su.SpawnBots = 0"));
		return;
	}

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

void ASGameModeBase::HandlePawnKilled(APawn* KilledPawn, APawn* InstigatorPawn)
{
	AController* PawnController = KilledPawn->GetController();

	if (APlayerController* PlayerController = Cast<APlayerController>(PawnController))
	{
		RespawnPlayer(PlayerController);
	}
	
	DisposeCorpse(KilledPawn);
}

void ASGameModeBase::RespawnPlayer(APlayerController* PlayerController)
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this, PlayerController]()
	{
		PlayerController->UnPossess();
		RestartPlayer(PlayerController);
	}, 3.0f, false);
}

void ASGameModeBase::DisposeCorpse(APawn* KilledPawn)
{
	FTimerHandle TimerHanlde;
	FTimerDelegate TimerDelegate;

	GetWorldTimerManager().SetTimer(TimerHanlde, TimerDelegate, 5.0f, false);
	TimerDelegate.BindLambda([KilledPawn]()
	{
		if (KilledPawn)
		{
			KilledPawn->Destroy();
		}
	});
}

void ASGameModeBase::HandleSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	TArray<FVector> BotSpawnLocations = QueryInstance->GetResultsAsLocations();
	if (QueryStatus == EEnvQueryStatus::Type::Success && BotSpawnLocations.IsValidIndex(0))
	{
		FVector SpawnLocation = BotSpawnLocations[0];
		ASAICharacter* AICharacter = GetWorld()->SpawnActor<ASAICharacter>(BotCharacterClass, SpawnLocation, FRotator::ZeroRotator);

		// Handled in ASCharacterBase for both Player and Bots
		// AICharacter->OnPawnDied.AddLambda([this, AICharacter](AActor* InstigatorActor)
		// {
		// 	if (this)
		// 	{
		// 		this->HandlePawnKilled(AICharacter, Cast<APawn>(InstigatorActor));
		// 	}
		// });
	}

}
