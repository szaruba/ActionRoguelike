// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacterAnimInstance.h"

void USCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ActionComp = USActionComponent::GetFrom(GetOwningActor());
}

void USCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (ActionComp)
	{
		static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Effect.Stunned", true);
		bIsStunned = ActionComp->ActiveTags.HasTag(StunnedTag);
	}
}
