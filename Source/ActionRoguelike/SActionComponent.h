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
	UFUNCTION(BlueprintPure)
	static USActionComponent* GetFrom(AActor* Actor);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);
	UFUNCTION(BlueprintPure)
	bool HasActionClass(TSubclassOf<USAction> ActionClass) const;
	UFUNCTION(BlueprintCallable)
	void RemoveAction(USAction* Action);
	UFUNCTION(BlueprintCallable)
	void RemoveActionClass(TSubclassOf<USAction> ActionClass);
	UFUNCTION()
	bool StartActionByName(AActor* Instigator, const FName ActionName);
	UFUNCTION(Server, Reliable)
	void ServerStartActionByName(AActor* Instigator, const FName ActionName);
	UFUNCTION(BlueprintCallable)
	bool StopActionByName(AActor* Instigator, const FName ActionName);
	UFUNCTION(Server, Reliable)
	void ServerStopActionByName(AActor* Instigator, const FName ActionName);
	UFUNCTION()
	bool StopRunningActions(AActor* Instigator);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer ActiveTags;

	UPROPERTY(BlueprintAssignable)
	FOnTagsChanged OnTagsChanged;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<USAction>> DefaultActions;
	
private:
	UPROPERTY(Replicated)
	TArray<USAction*> Actions;
};
