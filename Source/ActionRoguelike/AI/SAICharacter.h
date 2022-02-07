// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAICharacter.generated.h"

class USWorldUserWidget;
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

	UPROPERTY(VisibleAnywhere)
	FName HitFlashMaterial_TimeOfHitParam;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USWorldUserWidget> HealthBarWidgetClass;
	UPROPERTY()
	USWorldUserWidget* HealthBarWidget;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	void SetTargetActor(AActor* TargetActor);

	UFUNCTION()
	void OnSeePawn(APawn* SeenPawn);
	UFUNCTION()
	void HandleHealthChanged(USAttributeComponent* OwningComp, AActor* InstigatorActor, float HealthNew, float HealthDelta);

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
