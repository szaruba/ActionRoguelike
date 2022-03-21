// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool ApplyDamage(AActor* InstigatorActor, AActor* DamagedActor, float Damage);
	static bool ApplyDirectionalDamage(AActor* InstigatorActor, AActor* DamagedActor, float Damage, const FHitResult& HitResult);

};
