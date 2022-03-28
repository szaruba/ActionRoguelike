// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"


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
	Credits += Amount;
	OnCreditsChanged.Broadcast(this, Credits, Amount);
}

bool ASPlayerState::RemoveCredits(int32 Amount)
{
	if (Credits < Amount)
	{
		return false;
	}

	Credits -= Amount;
	OnCreditsChanged.Broadcast(this, Credits, -Amount);
	return true;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

