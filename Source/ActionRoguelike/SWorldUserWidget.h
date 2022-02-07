// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "SWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn=true))
	AActor* AttachedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldOffset;

	USWorldUserWidget(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
