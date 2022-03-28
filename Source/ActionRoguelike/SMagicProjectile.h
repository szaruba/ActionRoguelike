// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectile.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:
	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> Hit_CameraShake;

	UPROPERTY(EditAnywhere)
	float Hit_CameraShake_InnerRadius;

	UPROPERTY(EditAnywhere)
	float Hit_CameraShake_OuterRadius;

	UPROPERTY(EditAnywhere)
	FGameplayTag ParryTag;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	
private:
	UFUNCTION()
	void HandleOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void HandleOnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void Explode() override;
};
