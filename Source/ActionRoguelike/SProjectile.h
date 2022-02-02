// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* ParticleSystemComp;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplodeParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	float MaxLifetime;

	UFUNCTION(BlueprintCallable)
	virtual void Explode();

public:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void OnMaxLifetimeExpired();

	UFUNCTION()
	virtual void HandleOnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
