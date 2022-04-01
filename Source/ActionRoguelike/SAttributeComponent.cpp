// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	RageMax = 100.f;
	Rage = 0.f;
	RageGainRate = 2.f;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{	
	AActor* OwnerActor = GetOwner();
	if (!ensure(OwnerActor) || !OwnerActor->CanBeDamaged() || !OwnerActor->HasAuthority())
	{
		return false;
	}
	
	float HealthBefore = Health;
	Health += Delta;
	Health = FMath::Clamp(Health, 0.f, HealthMax);
	float ActualDelta = Health - HealthBefore;

	if (ActualDelta != 0.f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}
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

void USAttributeComponent::ApplyRageChange(float Delta)
{
	float OldRageValue = Rage;
	Rage += Delta;
	Rage = FMath::Clamp(Rage, 0.f, RageMax);
	float ActualDelta = Rage - OldRageValue;
	if (ActualDelta != 0.f)
	{
		OnRageChanged.Broadcast(this, Rage, ActualDelta);
	}
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

float USAttributeComponent::GetRageMax() const
{
	return RageMax;
}

float USAttributeComponent::GetRageGainRate() const
{
	return RageGainRate;
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

/*
 * Replication
 */

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float HealthNew,
	float HealthDelta)
{
	OnHealthChanged.Broadcast(this, InstigatorActor, HealthNew, HealthDelta);
}


void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);
}

