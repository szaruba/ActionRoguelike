// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickup.h"
#include "GameFramework/Actor.h"
#include "SCoinPickup.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASCoinPickup : public ASPickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASCoinPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

private:
	UPROPERTY(EditAnywhere)
	int32 CreditAmount;
};
