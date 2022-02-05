// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_WithinAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_WithinAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!ensure(BBComp))
	{
		return;
	}

	bool bWithinAttackRange = false;

	AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (TargetActor)
	{
		APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
		if (ensure(AIPawn) && AIPawn->GetDistanceTo(TargetActor) < AttackRangeMax)
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			if (ensure(AIController))
			{
				if (AIController->LineOfSightTo(TargetActor))
				{
					bWithinAttackRange = true;
				}
			}
		}
	}
	
	BBComp->SetValueAsBool(WithinRangeKey.SelectedKeyName, bWithinAttackRange);
}
