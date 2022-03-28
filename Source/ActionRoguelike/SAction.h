// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "SAction.generated.h"

class USActionComponent;

/**
 * Action can only be started if it is not running already
 * Action can only be stopped if it is running.
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintNativeEvent)
	void StartAction(AActor* ActionInstigator, bool& bOutSuccess);
	
	UFUNCTION(BlueprintNativeEvent)
	void StopAction(AActor* ActionInstigator, bool& bOutSuccess);

	USActionComponent* GetOwningComponent() const;
	bool IsRunning() const;

protected:
	bool bIsRunning;
};
