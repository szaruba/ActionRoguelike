// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

void USAction::StartAction_Implementation(AActor* ActionInstigator)
{
	UE_LOG(LogTemp, Display, TEXT("Start Action %s"), *GetNameSafe(this));
}

void USAction::StopAction_Implementation(AActor* ActionInstigator)
{
	UE_LOG(LogTemp, Display, TEXT("Stop Action %s"), *GetNameSafe(this));
}
