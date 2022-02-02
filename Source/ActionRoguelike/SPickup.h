// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SPickup.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPickup : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPickup();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InactiveDuration;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CollisionComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;
	
	virtual void BeginPlay() override;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartInactiveDuration();
	void SetActive(const bool bIsActive);
};
