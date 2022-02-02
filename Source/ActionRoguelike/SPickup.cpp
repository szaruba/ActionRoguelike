// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickup.h"


// Sets default values
ASPickup::ASPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>("CollisionComp");
	SetRootComponent(CollisionComp);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(CollisionComp);

	InactiveDuration = 10.f;
}

// Called when the game starts or when spawned
void ASPickup::BeginPlay()
{
	Super::BeginPlay();
}

void ASPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in sub-classes
}

// Called every frame
void ASPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASPickup::StartInactiveDuration()
{
	SetActive(false);
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ASPickup::SetActive, true);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, InactiveDuration, false);
}

void ASPickup::SetActive(const bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
	SetActorEnableCollision(bIsActive);
	SetActorTickEnabled(bIsActive);
}

