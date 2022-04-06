// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;
class ASPlayerState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, ChangedInState, int32, NewAmount, int32, Delta);


UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASPlayerState();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AddCredits(int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	bool RemoveCredits(int32 Amount);

	UFUNCTION(BlueprintPure)
	int32 GetCredits() const;

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

	void SavePlayerState(USSaveGame* SG);
	void LoadPlayerState(USSaveGame* SG);

protected:
	UFUNCTION(Server, Reliable)
	void ServerAddCredits(int32 Amount);

	UFUNCTION(Server, Reliable)
	void ServerRemoveCredits(int32 Amount);
	
private:
	UPROPERTY(ReplicatedUsing="OnRep_Credits")
	int32 Credits;
	UFUNCTION()
	void OnRep_Credits(int32 CreditsOld);
};
