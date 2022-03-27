// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SBlackholeProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASBlackholeProjectile : public ASProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASBlackholeProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	URadialForceComponent* RadialForceComp;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
