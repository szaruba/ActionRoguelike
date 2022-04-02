// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"


USAction::USAction()
{
	RageActivationCost = 0.f;
}

void USAction::StartAction_Implementation(AActor* ActionInstigator)
{
	UE_LOG(LogTemp, Display, TEXT("Start Action %s"), *GetNameSafe(this));

	if (USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(ActionInstigator))
	{
		Attributes->ApplyRageChange(-RageActivationCost);
	}
	
	GetOwningComponent()->ActiveTags.AppendTags(GrantedTags);
	
	RepData.bIsRunning = true;
	RepData.Instigator = ActionInstigator;
}

void USAction::StopAction_Implementation(AActor* ActionInstigator)
{
	UE_LOG(LogTemp, Display, TEXT("Stop Action %s"), *GetNameSafe(this));
	GetOwningComponent()->ActiveTags.RemoveTags(GrantedTags);
	
	RepData.bIsRunning = false;
	RepData.Instigator = ActionInstigator;
}

bool USAction::CanStart_Implementation(AActor* ActionInstigator) const
{
	if (RepData.bIsRunning)
	{
		return false;
	}

	if (!GetOwningComponent() || GetOwningComponent()->ActiveTags.HasAnyExact(BlockedTags))
	{
		return false;
	}

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

	if (Attributes && Attributes->GetRage() < RageActivationCost)
	{
		return false;
	}

	return true;
}

USActionComponent* USAction::GetOwningComponent() const
{
	AActor* OwningActor = GetTypedOuter<AActor>();
	return USActionComponent::GetFrom(OwningActor);
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool USAction::GetAutoStart() const
{
	return bAutoStart;
}

UWorld* USAction::GetWorld() const
{
	if (AActor* OwningActor = GetTypedOuter<AActor>())
	{
		return OwningActor->GetWorld();
	}
	return nullptr;
}

void USAction::OnRep_RepData()
{
	UE_LOG(LogTemp, Warning, TEXT("Replicating!!"));
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
}