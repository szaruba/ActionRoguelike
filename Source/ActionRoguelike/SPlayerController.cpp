// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "Blueprint/UserWidget.h"


void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPossessDelegate.Broadcast(InPawn);
}

void ASPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
}

void ASPlayerController::TogglePauseMenu()
{
	ensure(PauseMenuWidgetClass);
	
	if (!PauseMenuWidget)
	{
		PauseMenuWidget = CreateWidget(this, PauseMenuWidgetClass);
		PauseMenuWidget->AddToViewport(100);
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
	}
	else
	{
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", EInputEvent::IE_Pressed, this, &ASPlayerController::TogglePauseMenu);
}

