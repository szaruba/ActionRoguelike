// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "UObject/Object.h"
#include "SAction_ProjectileAttack.generated.h"

class ASProjectile;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

public:
	USAction_ProjectileAttack();
	virtual void StartAction_Implementation(AActor* ActionInstigator, bool& bOutSuccess);
	virtual void StopAction_Implementation(AActor* ActionInstigator, bool& bOutSuccess);

protected:
	void SpawnProjectile(ACharacter* Instigator);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SpawnSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnDelay;
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* AttackAnim;
	UPROPERTY(EditAnywhere)
	UParticleSystem* Particles;

	FTimerHandle TimerHandle_SpawnProjectile;
};
