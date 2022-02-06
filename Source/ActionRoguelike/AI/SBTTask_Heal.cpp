// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTask_Heal.h"

#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"

USBTTask_Heal::USBTTask_Heal()
{
	HealAmount = 50.f;
}

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		APawn* OwnerPawn = AIController->GetPawn();
		if (OwnerPawn)
		{
			USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(OwnerPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (AttributeComponent)
			{
				AttributeComponent->ApplyHealthChange(OwnerPawn, HealAmount);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
