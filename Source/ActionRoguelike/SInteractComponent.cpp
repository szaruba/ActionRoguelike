// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractComponent.h"

#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"


// Sets default values for this component's properties
USInteractComponent::USInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InteractionRange = 300.f;
	InteractionSphereRadius = 50.f;
}

void USInteractComponent::PrimaryInteract() const
{
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	if (TargetActor && ensure(InstigatorPawn))
	{
		ISGameplayInterface::Execute_Interact(TargetActor, InstigatorPawn);
	}
	
}


// Called when the game starts
void USInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}


void USInteractComponent::ScanForTargetActor()
{
	TargetActor = nullptr;
	
	APawn* OwnerPawn = GetOwner<APawn>();
	if (!OwnerPawn)
	{
		return;
	}
	
	TArray<FHitResult> Hits;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(InteractionSphereRadius);
	FVector ViewPoint;
	FRotator ViewRotation;
	OwnerPawn->GetController()->GetPlayerViewPoint(ViewPoint, ViewRotation);
	FVector SweepEnd = ViewPoint + ViewRotation.Vector() * InteractionRange * 2;
	GetWorld()->SweepMultiByObjectType(Hits, ViewPoint, SweepEnd, FQuat::Identity, ObjectQueryParams, CollisionShape);
	
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor->Implements<USGameplayInterface>() && OwnerPawn->GetDistanceTo(HitActor) <= InteractionRange)
		{
			TargetActor = HitActor;
			break;
		}
		
		if (Hit.bBlockingHit)
		{
			break;
		}
	}
}

// Called every frame
void USInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ScanForTargetActor();

	// Handle Overlay Widget
	if (TargetActor)
	{
		if (!OverlayWidget && ensure(OverlayWidgetClass))
		{
			OverlayWidget = CreateWidget<USWorldUserWidget>(GetWorld(), OverlayWidgetClass);
		}
		
		OverlayWidget->AttachedActor = TargetActor;

		if (!OverlayWidget->IsInViewport())
		{
			OverlayWidget->AddToViewport();
		}
	}
	if (!TargetActor && OverlayWidget && OverlayWidget->IsInViewport())
	{
		OverlayWidget->RemoveFromParent();
	}
}

