// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWorldUserWidget.h"
#include "Components/ActorComponent.h"
#include "SInteractComponent.generated.h"



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USInteractComponent();

	void PrimaryInteract() const;

protected:
	UPROPERTY(EditAnywhere)
	float InteractionRange;

	UPROPERTY(EditAnywhere)
	float InteractionSphereRadius;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USWorldUserWidget> OverlayWidgetClass;

	UPROPERTY()
	USWorldUserWidget* OverlayWidget;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	void ScanForTargetActor();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

};
