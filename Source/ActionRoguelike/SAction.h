// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "SAction.generated.h"

class USActionComponent;

/**
 * Action can only be started if it is not running already and is not blocked by active BlockedTags in the owning comp
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartAction(AActor* ActionInstigator);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopAction(AActor* ActionInstigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool CanStart(AActor* ActionInstigator) const;

	UFUNCTION(BlueprintPure)
	USActionComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintPure)
	bool IsRunning() const;

	UFUNCTION(BlueprintPure)
	bool GetAutoStart() const;
	
protected:
	bool bIsRunning;

	UPROPERTY(EditAnywhere)
	bool bAutoStart;
};
