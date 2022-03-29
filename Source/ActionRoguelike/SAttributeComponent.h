// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SBTService_CheckHealth.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

class USAttributeComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, USAttributeComponent*, OwningComp, AActor*, InstigatorActor, float, HealthNew, float, HealthDelta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRageChanged, USAttributeComponent*, AttributeComp, float, NewValue, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static USAttributeComponent* GetAttributes(AActor* OfActor);

	UFUNCTION(BlueprintPure)
	static bool IsActorAlive(AActor* Actor);
	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;

	UFUNCTION(BlueprintPure)
	bool IsFullHealth() const;
	UFUNCTION(BlueprintPure)
	float GetHealth() const;
	UFUNCTION(BlueprintPure)
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
	void ApplyRageChange(float Delta);
	UFUNCTION(BlueprintPure)
	float GetRage() const;
	UFUNCTION(BlueprintPure)
	float GetRageMax() const;
	UFUNCTION(BlueprintPure)
	float GetRageGainRate() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthMax;

	UPROPERTY(EditAnywhere)
	float Rage;
	UPROPERTY(EditAnywhere)
	float RageMax;
	// How many points of rage are gained per health point lost
	UPROPERTY(EditAnywhere)
	float RageGainRate;
};
