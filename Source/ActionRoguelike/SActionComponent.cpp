// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include <string>


// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();	
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	USAction* Action = NewObject<USAction>(this, ActionClass);
	Actions.Emplace(Action);
}

bool USActionComponent::StartActionByName(AActor* Instigator, const FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action->Name == ActionName)
		{
			if (ActiveActionTags.HasAnyExact(Action->BlockedTags))
			{
				UE_LOG(LogTemp, Display, TEXT("Action %s is blocked by an active action"), *GetNameSafe(this));
				return false;
			}
			bool bSuccess;
			Action->StartAction(Instigator, bSuccess);
			return bSuccess;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, const FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action->Name == ActionName)
		{
			bool bSuccess;
			Action->StopAction(Instigator, bSuccess);
			return bSuccess;
		}
	}
	return false;
}

bool USActionComponent::StopRunningActions(AActor* Instigator)
{
	for (USAction* Action : Actions)
	{
		if (Action->IsRunning())
		{
			bool bSuccess;
			Action->StopAction(Instigator, bSuccess);
		}
	}
	return true;
}

