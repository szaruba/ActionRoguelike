// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "UObject/Object.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:
	USActionEffect();
	
	virtual void StartAction_Implementation(AActor* ActionInstigator) override;
	virtual void StopAction_Implementation(AActor* ActionInstigator) override;

	UFUNCTION(BlueprintNativeEvent)
	void ExecutePeriodicEffect(AActor* ActionInstigator);

	UFUNCTION(BlueprintPure)
	float GetDuration() const;

	UFUNCTION(BlueprintPure)
	float GetTimeRemaining() const;
	
protected:
	UPROPERTY(EditAnywhere)
	float Period;
	UPROPERTY(EditAnywhere)
	float Duration;

private:
	FTimerHandle TimerHandle_Period;
	FTimerHandle TimerHandle_Duration;
};
