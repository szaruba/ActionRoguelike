// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

#include "SCharacterBase.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASItemChest::ASItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	bLidOpen = false;

	bReplicates = true;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if (bLidOpen)
	{
		bLidOpen = false;
		OnRep_LidOpen();
	}
	else
	{
		USActionComponent* ActionComp = USActionComponent::GetFrom(InstigatorPawn);
		if (!ActionComp || ActionComp->ActiveTags.HasAllExact(RequiredTags))
		{
			bLidOpen = true;
			OnRep_LidOpen();
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

void ASItemChest::OnRep_LidOpen()
{
	if (bLidOpen)
	{
		LidMesh->SetRelativeRotation(FRotator(MaxLidAngle, 0, 0));
	}
	else
	{
		LidMesh->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpen);
}

void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpen();
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASItemChest::GetLidOpen() const
{
	return bLidOpen;
}

