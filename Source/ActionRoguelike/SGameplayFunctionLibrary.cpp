// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

static TAutoConsoleVariable<float> CVarDirectionalDamageForce(TEXT("su.DirectionalDamageForce"), 300000.f,
	TEXT("Force applied on projectile impacts"), ECVF_Cheat);

bool USGameplayFunctionLibrary::ApplyDamage(AActor* InstigatorActor, AActor* DamagedActor, float Damage)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(DamagedActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(InstigatorActor, -Damage);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* InstigatorActor, AActor* DamagedActor, float Damage,
	const FHitResult& HitResult)
{
	if (ApplyDamage(InstigatorActor, DamagedActor, Damage))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			float ImpactForce = CVarDirectionalDamageForce.GetValueOnGameThread();
			HitComponent->AddImpulseAtLocation(-HitResult.ImpactNormal * ImpactForce, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
