// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"


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
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* ActionInstigator)
{
	UE_LOG(LogTemp, Display, TEXT("Stop Action %s"), *GetNameSafe(this));
	GetOwningComponent()->ActiveTags.RemoveTags(GrantedTags);
	bIsRunning = false;
}

bool USAction::CanStart_Implementation(AActor* ActionInstigator) const
{
	if (bIsRunning)
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
	return Cast<USActionComponent>(GetOuter());
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

bool USAction::GetAutoStart() const
{
	return bAutoStart;
}
