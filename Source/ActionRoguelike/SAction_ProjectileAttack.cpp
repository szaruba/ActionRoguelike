// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "SCharacter.h"
#include "SProjectile.h"
#include "Kismet/GameplayStatics.h"


USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	SpawnSocketName = "Muzzle_01";
	SpawnDelay = 0.2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* ActionInstigator)
{
	Super::StartAction_Implementation(ActionInstigator);

	ACharacter* InstigatorCharacter = Cast<ACharacter>(ActionInstigator);
	if (!InstigatorCharacter)
	{
		return;
	}
	
	UGameplayStatics::SpawnEmitterAttached(Particles, InstigatorCharacter->GetMesh(), SpawnSocketName);
	InstigatorCharacter->PlayAnimMontage(AttackAnim);

	if (ActionInstigator->HasAuthority())
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &USAction_ProjectileAttack::SpawnProjectile, InstigatorCharacter);
		InstigatorCharacter->GetWorldTimerManager().SetTimer(TimerHandle_SpawnProjectile, TimerDelegate, SpawnDelay, false);
	}
}

void USAction_ProjectileAttack::StopAction_Implementation(AActor* ActionInstigator)
{
	Super::StopAction_Implementation(ActionInstigator);
	
	ActionInstigator->GetWorldTimerManager().ClearTimer(TimerHandle_SpawnProjectile);
}

void USAction_ProjectileAttack::SpawnProjectile(ACharacter* Instigator)
{
	if (!ensure(ProjectileClass))
	{
		StopAction(Instigator);
		return;
	}
	
	AController* PlayerController = Instigator->GetController();
	if (!PlayerController)
	{
		StopAction(Instigator);
		return;
	}
	FHitResult Hit;
	FVector CameraLocation;
	FRotator CameraRot;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRot);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Instigator);
	FVector TraceEnd = CameraLocation + 100000 * CameraRot.Vector();
	GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, TraceEnd, FCollisionObjectQueryParams::AllObjects, QueryParams);

	FVector HitLocation = Hit.IsValidBlockingHit() ? Hit.Location : TraceEnd;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = Instigator;
	FVector HandLocation = Instigator->GetMesh()->GetSocketLocation(SpawnSocketName);
	FRotator ProjectileRot = (HitLocation - HandLocation).Rotation();
	FTransform SpawnTM = FTransform(ProjectileRot, HandLocation);
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

	StopAction(Instigator);
}
