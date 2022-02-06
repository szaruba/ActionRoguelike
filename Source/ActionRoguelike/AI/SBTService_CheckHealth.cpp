// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckHealth.h"

#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckHealth::USBTService_CheckHealth()
{
	LowHealthThreshold = 50.f;
}

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (OwnerPawn)
	{
		USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(OwnerPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensureMsgf(AttributeComponent, TEXT("Assign an USAttributeComponent to the AIPawn!")))
		{
			bool bHasLowHealth = AttributeComponent->GetHealth() <= LowHealthThreshold;
			UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
			if (ensureMsgf(BBComp, TEXT("Assign a BlackboardComponent to the BehaviorTreeComponent")))
			{
				BBComp->SetValueAsBool(HasLowHealthKey.SelectedKeyName, bHasLowHealth);
			}
		}
	}
}
