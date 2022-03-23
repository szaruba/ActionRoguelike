// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionComponent.h"
#include "GameFramework/Character.h"
#include "SCharacterBase.generated.h"

class USAttributeComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPawnDied, AActor*);

UCLASS()
class ACTIONROGUELIKE_API ASCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	FOnPawnDied OnPawnDied;
	
	// Sets default values for this character's properties
	ASCharacterBase();

protected:
	UPROPERTY(EditAnywhere)
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USActionComponent* ActionComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void HandleHealthChanged(USAttributeComponent* OwningComp, AActor* InstigatorActor, float HealthNew, float HealthDelta);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;
};
