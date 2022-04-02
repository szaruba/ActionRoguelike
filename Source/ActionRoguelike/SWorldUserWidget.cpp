// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"


USWorldUserWidget::USWorldUserWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	WorldOffset = FVector::ZeroVector;
}

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		FVector2D ScreenPosition;
		FVector ActorLocation = AttachedActor->GetActorLocation();

		bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(PlayerController, ActorLocation + WorldOffset, ScreenPosition);

		if (bIsOnScreen)
		{
			float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);
			ParentSizeBox->SetRenderTranslation(ScreenPosition / ViewportScale);
		}
	}
}
