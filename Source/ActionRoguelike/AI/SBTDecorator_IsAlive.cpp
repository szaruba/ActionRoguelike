// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTDecorator_IsAlive.h"

#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

bool USBTDecorator_IsAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BBComp))
	{
		AActor* Actor = Cast<AActor>(BBComp->GetValueAsObject(ActorKey.SelectedKeyName));
		if (Actor)
		{
			return USAttributeComponent::IsActorAlive(Actor);
		}
	}
	return false;
}
