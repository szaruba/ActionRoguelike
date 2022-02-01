// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"


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
	TArray<FHitResult> Hits;
	FVector EyesLocation;
	FRotator EyesRotation;
	GetOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotation);
	FVector TraceEnd = EyesLocation + EyesRotation.Vector() * InteractionRange;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(InteractionSphereRadius);
	
	GetWorld()->SweepMultiByObjectType(Hits, EyesLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, CollisionShape);
	DrawDebugLine(GetWorld(), EyesLocation, TraceEnd, FColor::Blue, false, 2.f, 0, 2.f);
	
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		APawn* InstigatorPawn = Cast<APawn>(GetOwner());
		if (HitActor->Implements<USGameplayInterface>() && InstigatorPawn)
		{
			ISGameplayInterface::Execute_Interact(HitActor, InstigatorPawn);

			DrawDebugSphere(GetWorld(), Hit.Location, InteractionSphereRadius, 32, FColor::Green, false, 2.f, 0, 2.f);
			break;
		}
		else
		{
			DrawDebugSphere(GetWorld(), Hit.Location, InteractionSphereRadius, 32, FColor::Red, false, 2.f, 0, 2.f);
		}
		
		if (Hit.bBlockingHit)
		{
			break;
		}
	}
}


// Called when the game starts
void USInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

