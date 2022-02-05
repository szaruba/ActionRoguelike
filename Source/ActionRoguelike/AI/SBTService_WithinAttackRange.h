// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_WithinAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_WithinAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector WithinRangeKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	float AttackRangeMax;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
