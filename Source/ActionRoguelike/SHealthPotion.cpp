// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SInteractComponent.h"
#include "SPlayerState.h"


#define LOCTEXT_NAMESPACE "InteractableActors"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealAmount = 50.f;
	CreditCost = 100;
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
		USAttributeComponent* AttrComp = USAttributeComponent::GetAttributes(InstigatorPawn);
		
		if (AttrComp && !AttrComp->IsFullHealth())
		{
			// if Instigator is a player check if they have enough credits to use potion
			if (ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>())
			{
				if (!PlayerState->RemoveCredits(CreditCost))
				{
					return;
				}
			}
			
			AttrComp->ApplyHealthChange(this, HealAmount);
			StartInactiveDuration();
		}
	}
	
}

FText ASHealthPotion::GetInteractionDetailMessage_Implementation(APawn* InstigatorPawn) const
{
	USAttributeComponent* AttrComp = USAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttrComp && AttrComp->IsFullHealth())
	{
		return FText(LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health!"));
	}
	if (ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		if (PlayerState->GetCredits() < CreditCost)
		{
			return FText::Format(LOCTEXT("HealthPotion_NotEnoughCreditsWarning", "Need {0} credits to buy this potion."), CreditCost);
		}
	}
	return FText::GetEmpty();
}

