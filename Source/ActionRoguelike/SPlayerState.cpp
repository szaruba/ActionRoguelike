// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASPlayerState::ASPlayerState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Credits = 0;
}

// Called when the game starts or when spawned
void ASPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASPlayerState::AddCredits(int32 Amount)
{
	// if (!HasAuthority())
	// {
	// 	ServerAddCredits(Amount);
	// 	return;
	// }
	
	Credits += Amount;
	OnCreditsChanged.Broadcast(this, Credits, Amount);
}

void ASPlayerState::ServerAddCredits_Implementation(int32 Amount)
{
	AddCredits(Amount);
}

bool ASPlayerState::RemoveCredits(int32 Amount)
{
	if (Credits < Amount)
	{
		return false;
	}
	
	// if (!HasAuthority())
	// {
	// 	ServerRemoveCredits(Amount);
	// 	return true;
	// }

	Credits -= Amount;
	OnCreditsChanged.Broadcast(this, Credits, -Amount);
	return true;
}

void ASPlayerState::ServerRemoveCredits_Implementation(int32 Amount)
{
	RemoveCredits(Amount);
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::SavePlayerState(USSaveGame* SG)
{
	if (ensureAlways(SG))
	{
		SG->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState(USSaveGame* SG)
{
	if (ensure(SG))
	{
		Credits = SG->Credits;
	}
}

void ASPlayerState::OnRep_Credits(int32 CreditsOld)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - CreditsOld);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}

