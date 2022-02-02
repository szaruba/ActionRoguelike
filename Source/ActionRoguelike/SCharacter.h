// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USInteractComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClassPrimary;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClassSecondary;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClassTeleport;
	
	UPROPERTY(VisibleAnywhere)
	USInteractComponent* InteractComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* PrimaryAttackAnim;
	UPROPERTY(EditAnywhere)
	UParticleSystem* PrimaryAttack_Particles;

	void MoveForward(float value);
	void MoveRight(float value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PrimaryAttack();
	void SecondaryAttack();
	void SpawnProjectile(TSubclassOf<AActor> ProjectileClass);
	void PrimaryInteract();
	void Teleport();

	UFUNCTION()
	void OnHealthChanged(USAttributeComponent* OwningComp, AActor* InstigatorActor, float HealthNew, float HealthDelta);

	virtual void PostInitializeComponents() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
