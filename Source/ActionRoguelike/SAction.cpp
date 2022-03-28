// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* ActionInstigator, bool& bOutSuccess)
{
	if (!bIsRunning)
	{
		UE_LOG(LogTemp, Display, TEXT("Start Action %s"), *GetNameSafe(this));
		GetOwningComponent()->ActiveActionTags.AppendTags(GrantedTags);
		bIsRunning = true;
		bOutSuccess = true;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Action %s is already running."), *GetNameSafe(this));
		bOutSuccess = false;
	}
}

void USAction::StopAction_Implementation(AActor* ActionInstigator, bool& bOutSuccess)
{
	if (bIsRunning)
	{
		UE_LOG(LogTemp, Display, TEXT("Stop Action %s"), *GetNameSafe(this));
		GetOwningComponent()->ActiveActionTags.RemoveTags(GrantedTags);
		bIsRunning = false;
		bOutSuccess = true;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Action %s was not running"), *GetNameSafe(this));
		bOutSuccess = false;
	}
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}
