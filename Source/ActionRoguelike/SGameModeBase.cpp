// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SCoinPickup.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameData/FSMonsterInfoRow.h"
#include "GameData/SMonsterData.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable/Disable bot spawning"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarSpawnCoins = TAutoConsoleVariable<bool>(TEXT("su.SpawnCoins"), true, TEXT("Should spawn coins periodically"));

ASGameModeBase::ASGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	BotSpawnRate = 5.f;
	CoinSpawnRate = 3.f;

	PlayerStateClass = ASPlayerState::StaticClass();

	SlotName = "SaveGame01";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (ASPlayerState* PlayerState = Cast<ASPlayerState>(NewPlayer->PlayerState))
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
}

void ASGameModeBase::WriteSaveGame()
{
	if (!ensure(CurrentSaveGame))
	{
		return;
	}

	CurrentSaveGame->SavedActors.Empty();
	CurrentSaveGame->SavedPlayers.Empty();
	
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (ASPlayerState* SPlayerState = Cast<ASPlayerState>(PlayerState))
		{
			SPlayerState->SavePlayerState(CurrentSaveGame);
			break;
		}
	}

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FSaveDataActor SaveDataActor;
		SaveDataActor.Name = Actor->GetFName();
		SaveDataActor.Transform = Actor->GetTransform();

		
		FMemoryWriter MemWriter(SaveDataActor.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		
		CurrentSaveGame->SavedActors.Add(SaveDataActor);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (USSaveGame* LoadedSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)))
	{
		CurrentSaveGame = LoadedSaveGame;
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
	}

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		for (FSaveDataActor SaveDataActor : CurrentSaveGame->SavedActors)
		{
			if (Actor->GetFName() == SaveDataActor.Name)
			{
				Actor->SetActorTransform(SaveDataActor.Transform);

				FMemoryReader MemReader(SaveDataActor.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				Ar.ArIsSaveGame = true;
				Actor->Serialize(Ar);
				ISGameplayInterface::Execute_OnActorLoaded(Actor);
				break;
			}
		}
		
	}
}

void ASGameModeBase::SpawnBots(int32 Amount)
{
	bool bShouldSpawn = CVarSpawnBots.GetValueOnGameThread();
	CVarSpawnBots->Set(true);
	for (int32 i = 0; i < Amount; ++i)
	{
		RunSpawnBotQuery();
	}
	CVarSpawnBots->Set(bShouldSpawn);
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

	FTimerHandle TimerHandle_BotSpawn;
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawn, this, &ASGameModeBase::RunSpawnBotQuery, BotSpawnRate, true);
	FTimerHandle TimerHandle_CoinSpawn;
	GetWorldTimerManager().SetTimer(TimerHandle_CoinSpawn, this, &ASGameModeBase::SpawnCoin, CoinSpawnRate, true);
}

void ASGameModeBase::SpawnCoin()
{
	if (!CVarSpawnCoins.GetValueOnGameThread())
	{
		return;
	}
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, EnvQuery_FindCoinSpawn, this, EEnvQueryRunMode::AllMatching, nullptr);
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::HandleSpawnCoinQueryFinished);
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

	// If killer is a player, then grant them reward credits for the kill.
	if (InstigatorPawn)
	{
		if (ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			PlayerState->AddCredits(100);
		}
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

void ASGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// for(APlayerState* PlayerState : GetGameState<AGameStateBase>()->PlayerArray)
	// {
	// 	if (ASPlayerState* SPlayerState = Cast<ASPlayerState>(PlayerState))
	// 	{
	// 		SPlayerState->AddCredits(1);
	// 	}
	// }
}


void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId MonsterId, FVector SpawnLocation)
{
	if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
	{
		USMonsterData* MonsterData = Cast<USMonsterData>(AssetManager->GetPrimaryAssetObject(MonsterId));

		ASAICharacter* Bot = GetWorld()->SpawnActor<ASAICharacter>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
		USActionComponent* ActionComp = USActionComponent::GetFrom(Bot);
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
	
		for (TSubclassOf<USAction> ActionClass : MonsterData->Actions)
		{
			ActionComp->AddAction(this, ActionClass);
		}

		AttributeComp->HealthMax = MonsterData->MaxHealth;
		AttributeComp->Health = MonsterData->MaxHealth;
		Bot->Level = MonsterData->MonsterLevel;
		if (MonsterData->MonsterLevel > 15)
		{
			Bot->SetActorScale3D(FVector(2.f, 2.f, 2.f));
		}
	}
}

void ASGameModeBase::HandleSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	TArray<FVector> BotSpawnLocations = QueryInstance->GetResultsAsLocations();
	if (QueryStatus == EEnvQueryStatus::Type::Success && BotSpawnLocations.IsValidIndex(0))
	{
		FVector SpawnLocation = BotSpawnLocations[0];

		if (ensure(MonsterTable))
		{
			TArray<FSMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);
			FSMonsterInfoRow* MonsterInfo = Rows[FMath::RandRange(0, Rows.Num()-1)];

			FPrimaryAssetId AssetId = MonsterInfo->MonsterId;
			if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
			{
				FStreamableDelegate OnLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnMonsterLoaded, AssetId, SpawnLocation);
				AssetManager->LoadPrimaryAsset(AssetId, TArray<FName>(), OnLoadedDelegate);
			}
		}
	}
}

void ASGameModeBase::HandleSpawnCoinQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	TArray<FVector> SpawnLocations;
	QueryInstance->GetQueryResultsAsLocations(SpawnLocations);
	if (QueryStatus == EEnvQueryStatus::Type::Success && SpawnLocations.Num() > 0)
	{
		FVector SpawnLocation = SpawnLocations[FMath::RandRange(0, SpawnLocations.Num()-1)];

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		const FTransform SpawnTM = FTransform(SpawnLocation);
		GetWorld()->SpawnActor(SpawnedCoinClass, &SpawnTM, SpawnParameters);
	}
}
