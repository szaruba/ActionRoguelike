// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoinPickup.h"

#include "SPlayerState.h"


// Sets default values
ASCoinPickup::ASCoinPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreditAmount = 50;
}

// Called when the game starts or when spawned
void ASCoinPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCoinPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCoinPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	if(ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PlayerState->AddCredits(CreditAmount);
	}

	Destroy();
}

