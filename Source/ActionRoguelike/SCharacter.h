// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeComponent.h"
#include "SCharacterBase.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USInteractComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ASCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

	UFUNCTION(Exec)
	void Heal(float Amount = 100.0f);

	UFUNCTION(Exec)
	void Kill(AActor* Target = nullptr);

	UFUNCTION(Exec)
	void God(bool bEnabled = true);

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClassSecondary;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClassTeleport;
	
	UPROPERTY(VisibleAnywhere)
	USInteractComponent* InteractComp;


	void MoveForward(float value);
	void MoveRight(float value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PrimaryAttack();
	void SecondaryAttack();

	void PrimaryInteract();
	void Teleport();
	void StartSprint();
	void StopSprint();
	
	virtual void HandleHealthChanged(USAttributeComponent* OwningComp, AActor* InstigatorActor, float HealthNew, float HealthDelta) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Parry();
	void CancelActions();
};
