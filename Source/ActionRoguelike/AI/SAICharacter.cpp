// Fill out your copyright notice in the Description page of Project Settings.


#include "SAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
}

// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);
}

void ASAICharacter::OnSeePawn(APawn* SeenPawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
		BBComp->SetValueAsObject("TargetActor", SeenPawn);
	}
}

// Called every frame
void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

