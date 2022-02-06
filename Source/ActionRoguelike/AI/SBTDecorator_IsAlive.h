// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SBTDecorator_IsAlive.generated.h"

/**
 * Checks if the Actor is alive or not
 */
UCLASS()
class ACTIONROGUELIKE_API USBTDecorator_IsAlive : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ActorKey;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
