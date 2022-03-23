// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UFUNCTION(BlueprintNativeEvent)
	void StartAction(AActor* ActionInstigator);
	
	UFUNCTION(BlueprintNativeEvent)
	void StopAction(AActor* ActionInstigator);
};
