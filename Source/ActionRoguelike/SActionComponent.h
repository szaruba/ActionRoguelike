// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTagsChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USActionComponent();

protected:
	virtual void BeginPlay() override;

public:
	static USActionComponent* GetFrom(AActor* Actor);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);
	UFUNCTION(BlueprintCallable)
	void RemoveAction(USAction* Action);
	UFUNCTION()
	bool StartActionByName(AActor* Instigator, const FName ActionName);
	UFUNCTION()
	bool StopActionByName(AActor* Instigator, const FName ActionName);
	UFUNCTION()
	bool StopRunningActions(AActor* Instigator);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer ActiveTags;

	UPROPERTY(BlueprintAssignable)
	FOnTagsChanged OnTagsChanged;

private:
	UPROPERTY()
	TArray<USAction*> Actions;
};
