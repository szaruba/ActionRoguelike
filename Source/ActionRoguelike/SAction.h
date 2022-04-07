// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "SAction.generated.h"

class USActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionStopped);

/**
 * Action can only be started if it is not running already and is not blocked by active BlockedTags in the owning comp
 * Action can only be stopped if it is running.
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:
	USAction();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;

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

	virtual UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	UFUNCTION(BlueprintPure)
	float GetTimeStarted() const;

	UPROPERTY(BlueprintAssignable)
	FOnActionStopped OnActionStopped;
	
protected:
	UPROPERTY(EditAnywhere)
	bool bAutoStart;

	UPROPERTY(EditAnywhere)
	float RageActivationCost;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	float TimeStarted;
};