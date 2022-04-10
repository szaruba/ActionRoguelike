// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickup.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASHealthPotion();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealAmount;

	UPROPERTY(EditAnywhere)
	int32 CreditCost;
	
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractionDetailMessage_Implementation(APawn* InstigatorPawn) const override;
};
