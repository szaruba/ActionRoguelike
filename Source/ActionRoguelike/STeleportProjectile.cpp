// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ASTeleportProjectile::ASTeleportProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxLifetime = 0.2f;
}

// Called when the game starts or when spawned
void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTeleportProjectile::TeleportInstigator(FVector Location)
{
	APawn* InstigatorPawn = GetInstigator();
	if(InstigatorPawn)
	{
		InstigatorPawn->SetActorLocation(Location);
	}
	Destroy();
}

void ASTeleportProjectile::OnMaxLifetimeExpired()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, DetonateEffect, GetActorLocation());
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ASTeleportProjectile::TeleportInstigator, GetActorLocation());
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.2f, false);
}

void ASTeleportProjectile::HandleOnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector HitLocation = Hit.Location;
	UGameplayStatics::SpawnEmitterAtLocation(this, DetonateEffect, HitLocation);
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ASTeleportProjectile::TeleportInstigator, HitLocation);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.2f, false);
}

