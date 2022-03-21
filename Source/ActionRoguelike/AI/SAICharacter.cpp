// Fill out your copyright notice in the Description page of Project Settings.


#include "SAICharacter.h"

#include "AIController.h"
#include "ActionRoguelike/SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	HitFlashMaterial_TimeOfHitParam = "TimeOfHit";
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

void ASAICharacter::SetTargetActor(AActor* TargetActor)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
		BBComp->SetValueAsObject("TargetActor", TargetActor);
	}
}

void ASAICharacter::OnSeePawn(APawn* SeenPawn)
{
	SetTargetActor(SeenPawn);
}

void ASAICharacter::HandleHealthChanged(USAttributeComponent* OwningComp, AActor* InstigatorActor, float HealthNew, float HealthDelta)
{
	if (HealthDelta < 0.f)
	{
		SetTargetActor(InstigatorActor);
		GetMesh()->SetScalarParameterValueOnMaterials(HitFlashMaterial_TimeOfHitParam, GetWorld()->GetTimeSeconds());

		if (!HealthBarWidget && ensure(HealthBarWidgetClass) && IsAlive())
		{
			HealthBarWidget = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			HealthBarWidget->AttachedActor = this;
			HealthBarWidget->AddToViewport();
		}
	}
	
	if (HealthNew <= 0.f)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->GetBrainComponent()->StopLogic("Killed");
		}
	}
}

// Called every frame
void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

