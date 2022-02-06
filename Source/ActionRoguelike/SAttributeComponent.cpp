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
	float HealthBefore = Health;
	Health += Delta;
	Health = FMath::Clamp(Health, 0.f, HealthMax);
	float ActualDelta = Health - HealthBefore;
	OnHealthChanged.Broadcast(this, InstigatorActor, Health, Delta);
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

