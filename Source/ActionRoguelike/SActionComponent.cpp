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
			Action->StartAction(Instigator);
			return true;
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
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

