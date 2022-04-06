// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossess, APawn*, PossessedPawn);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnPossess OnPossessDelegate;

	virtual void InitPlayerState() override;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

protected:
	UPROPERTY()
	UUserWidget* PauseMenuWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	virtual void SetupInputComponent() override;
};
