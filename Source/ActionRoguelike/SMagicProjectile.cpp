// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20.f;
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Hook OnOverlap
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::HandleOnOverlap);
}

void ASMagicProjectile::HandleOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -Damage);
		}
		DrawDebugSphere(GetWorld(), SweepResult.Location, CollisionComp->GetScaledSphereRadius(), 32, FColor::Red, false, 2.f, 0, 2);

		UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticles, SweepResult.Location);
		Destroy();
	}
}

void ASMagicProjectile::HandleOnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -Damage);
		}
		DrawDebugSphere(GetWorld(), Hit.Location, CollisionComp->GetScaledSphereRadius(), 64, FColor::Green, false, 2.f, 0, 2);

		UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticles, Hit.Location);
		Destroy();
	}
}

