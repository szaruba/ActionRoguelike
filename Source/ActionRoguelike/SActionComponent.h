// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USActionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddAction(TSubclassOf<USAction> ActionClass);
	UFUNCTION()
	bool StartActionByName(AActor* Instigator, const FName ActionName);
	UFUNCTION()
	bool StopActionByName(AActor* Instigator, const FName ActionName);
	UFUNCTION()
	bool StopRunningActions(AActor* Instigator);

	UPROPERTY()
	FGameplayTagContainer ActiveActionTags;

private:
	UPROPERTY()
	TArray<USAction*> Actions;
};
