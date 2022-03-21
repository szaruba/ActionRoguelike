// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor && !OwnerActor->CanBeDamaged())
	{
		return false;
	}
	float HealthBefore = Health;
	Health += Delta;
	Health = FMath::Clamp(Health, 0.f, HealthMax);
	float ActualDelta = Health - HealthBefore;
	OnHealthChanged.Broadcast(this, InstigatorActor, Health, ActualDelta);
	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* OfActor)
{
	if(OfActor)
	{
		return Cast<USAttributeComponent>(OfActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttrComp = GetAttributes(Actor);
	if (AttrComp)
	{
		return AttrComp->IsAlive();
	}
	return false;
}

