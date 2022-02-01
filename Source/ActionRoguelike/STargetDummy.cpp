﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"


// Sets default values
ASTargetDummy::ASTargetDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetRootComponent(MeshComp);

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	
}

// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::HandleOnHealthChanged);
}

// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTargetDummy::HandleOnHealthChanged(USAttributeComponent* OwningComp, AActor* InstigatorActor, float HealthNew,
	float HealthDelta)
{
	MeshComp->SetScalarParameterValueOnMaterials("TimeOfHit", GetWorld()->GetTimeSeconds());
}

