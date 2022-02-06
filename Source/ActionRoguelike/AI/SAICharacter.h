// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAICharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASAICharacter();

protected:
	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere)
	USAttributeComponent* AttributeComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnSeePawn(APawn* SeenPawn);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
