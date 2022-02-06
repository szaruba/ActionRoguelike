// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	BulletSpreadHalfAngle = 3.0f;
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ensure(BB) || !AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (TargetActor)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = AIPawn;

		FVector MuzzleLocation = AIPawn->GetActorLocation();
		FRotator SpawnRotation = (TargetActor->GetActorLocation() - MuzzleLocation).Rotation();
		SpawnRotation.Pitch += FMath::RandRange(0.f, BulletSpreadHalfAngle);
		SpawnRotation.Yaw += FMath::RandRange(-BulletSpreadHalfAngle, BulletSpreadHalfAngle);
		GetWorld()->SpawnActor<ASProjectile>(ProjectileClass, MuzzleLocation, SpawnRotation, SpawnParams);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
