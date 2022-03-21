// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacterBase.h"

#include "SAttributeComponent.h"
#include "SGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASCharacterBase::ASCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacterBase::HandleHealthChanged);
	OnPawnDied.AddLambda([this](AActor* InstigatorActor)
	{
		ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		GameMode->HandlePawnKilled(this, Cast<APawn>(InstigatorActor));
	});
}

void ASCharacterBase::HandleHealthChanged(USAttributeComponent* OwningComp, AActor* InstigatorActor, float HealthNew,
	float HealthDelta)
{
	if (HealthNew <= 0.f && HealthDelta < 0.f)
	{
		OnPawnDied.Broadcast(InstigatorActor);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();
		
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");
		SetLifeSpan(5.f);
	}
}

// Called every frame
void ASCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ASCharacterBase::IsAlive() const
{
	return AttributeComp->IsAlive();
}

