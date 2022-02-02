// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"


// Sets default values
ASHealthPotion::ASHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealAmount = 50.f;
}

// Called when the game starts or when spawned
void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		USAttributeComponent* AttrComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttrComp && !AttrComp->IsFullHealth())
		{
			AttrComp->ApplyHealthChange(this, HealAmount);
			StartInactiveDuration();
		}
	}
	
}

