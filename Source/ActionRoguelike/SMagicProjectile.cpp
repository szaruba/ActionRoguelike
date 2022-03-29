// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20.f;
	
	Hit_CameraShake_InnerRadius = 10000.f;
	Hit_CameraShake_OuterRadius = 20000.f;
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
		USActionComponent* ActionComponent = USActionComponent::GetFrom(OtherActor);
		if (ActionComponent && ActionComponent->ActiveTags.HasTagExact(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			// Apply burning effect
			if (ActionComponent)
			{
				ActionComponent->AddAction(GetInstigator(), BurningActionClass);
			}
			
			Explode();
		}
	}
}

void ASMagicProjectile::HandleOnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		if (USActionComponent* ActionComponent = USActionComponent::GetFrom(OtherActor))
		{
			if (ActionComponent->ActiveTags.HasTagExact(ParryTag))
			{
				MovementComp->Velocity = -MovementComp->Velocity;
				SetInstigator(Cast<APawn>(OtherActor));
				return;
			}
		}
		
		if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, Hit))
		{
			Explode();
		}
	}
}

void ASMagicProjectile::Explode()
{
	Super::Explode();
	// UGameplayStatics::PlayWorldCameraShake(this, Hit_CameraShake, GetActorLocation(), Hit_CameraShake_InnerRadius, Hit_CameraShake_OuterRadius);
}

