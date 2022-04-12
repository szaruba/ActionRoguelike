// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include <string>

#include "ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (const TSubclassOf<USAction> DefaultAction : DefaultActions)
		{
			AddAction(GetOwner(), DefaultAction);
		}
	}
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

	// for (USAction* Action : Actions)
	// {
	// 	FColor Color = Action->IsRunning() ? FColor::Blue : FColor::White;
	// 	FString Message = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
	// 		*GetNameSafe(GetOwner()),
	// 		*Action->Name.ToString(),
	// 		Action->IsRunning() ? TEXT("true") : TEXT("false"),
	// 		*GetNameSafe(Action->GetOuter()));
	// 	
	// 	LogOnScreen(this, Message, Color, 0.0f);
	// }
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	if(!ensure(ActionClass))
	{
		return;
	}
	
	USAction* Action = NewObject<USAction>(GetOwner(), ActionClass);
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
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	if(!ensure(Action && !Action->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(Action);
}

void USActionComponent::RemoveActionClass(TSubclassOf<USAction> ActionClass)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
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
			if (!GetOwner()->HasAuthority())
			{
				ServerStartActionByName(Instigator, ActionName);
			}
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
			if (!GetOwner()->HasAuthority())
			{
				ServerStopActionByName(Instigator, ActionName);
			}
			return true;
		}
	}
	return false;
}

USAction* USActionComponent::GetActionByClass(TSubclassOf<USAction> ActionClass)
{
	for (USAction* Action : Actions)
	{
		if (Action->IsA(ActionClass))
		{
			return Action;
		}
	}
	return nullptr;
}

void USActionComponent::ServerStartActionByName_Implementation(AActor* Instigator, const FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopActionByName_Implementation(AActor* Instigator, const FName ActionName)
{
	StopActionByName(Instigator, ActionName);
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

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (USAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}

