﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

#include "SCharacterBase.h"


// Sets default values
ASItemChest::ASItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if (USActionComponent* ActionComp = USActionComponent::GetFrom(InstigatorPawn))
	{
		if (ActionComp->ActiveTags.HasAllExact(RequiredTags))
		{
			LidMesh->SetRelativeRotation(FRotator(MaxLidAngle, 0, 0));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("You don't have all required tags to open the chest."));
		}
	}
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

