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

void USInteractComponent::PrimaryInteract()
{
	ServerPrimaryInteract();
}

void USInteractComponent::ServerPrimaryInteract_Implementation()
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
	if (!OwnerPawn || !OwnerPawn->GetController())
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

	if (GetOwner<APawn>()->IsLocallyControlled() || GetOwner<APawn>()->HasAuthority())
	{
		ScanForTargetActor();
	}
	
	if (GetOwner<APawn>()->IsLocallyControlled())
	{
		// Handle Overlay Widget
		if (TargetActor && !OverlayWidget && ensure(OverlayWidgetClass))
		{
			OverlayWidget = CreateWidget<USWorldUserWidget>(GetWorld(), OverlayWidgetClass);
			OverlayWidget->AttachedActor = TargetActor;
			OverlayWidget->AddToViewport();
		}
		else if (!TargetActor && OverlayWidget)
		{
			OverlayWidget->RemoveFromParent();
			OverlayWidget = nullptr;
		}
	}
}

