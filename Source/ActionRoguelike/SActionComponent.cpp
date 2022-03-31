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

USActionComponent* USActionComponent::GetFrom(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}
	
	return Cast<USActionComponent>(Actor->GetComponentByClass(StaticClass()));
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}
	
	USAction* Action = NewObject<USAction>(this, ActionClass);
	Actions.Emplace(Action);

	if (Action->GetAutoStart() && ensure(Action->CanStart(Instigator)))
	{
		Action->StartAction(Instigator);
	}
}

bool USActionComponent::HasActionClass(TSubclassOf<USAction> ActionClass) const
{
	return ContainsObjectOfClass(Actions, ActionClass);
}

void USActionComponent::RemoveAction(USAction* Action)
{
	if(!ensure(Action && !Action->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(Action);
}

void USActionComponent::RemoveActionClass(TSubclassOf<USAction> ActionClass)
{
	for (int32 Index = Actions.Num() - 1; Index >= 0; --Index)
	{
		if (Actions[Index]->IsA(ActionClass))
		{
			if (!Actions[Index]->IsRunning())
			{
				Actions.RemoveAt(Index);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Tried to remove a running action. Stop it before removing."));
			}
		}
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, const FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action->Name == ActionName && Action->CanStart(Instigator))
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
		if (Action->Name == ActionName && Action->IsRunning())
		{
			Action->StopAction(Instigator);
			return true;
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
			Action->StopAction(Instigator);
		}
	}
	return true;
}

