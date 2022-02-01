// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASProjectile::ASProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>("CollisionComp");
	CollisionComp->SetCollisionProfileName("Projectile");
	SetRootComponent(CollisionComp);

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComp");
	ParticleSystemComp->SetupAttachment(CollisionComp);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASProjectile::Explode_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticles, GetActorLocation());
	Destroy();
}

void ASProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	FHitResult HitResult;
	TimerDelegate.BindUObject(this, &ASProjectile::OnMaxLifetimeExpired);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, MaxLifetime, false);

	CollisionComp->OnComponentHit.AddDynamic(this, &ASProjectile::HandleOnActorHit);
}

void ASProjectile::OnMaxLifetimeExpired()
{
	Explode();
}

void ASProjectile::HandleOnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}
