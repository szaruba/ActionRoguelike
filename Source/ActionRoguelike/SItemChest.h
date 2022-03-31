// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASItemChest();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(EditAnywhere)
	float MaxLidAngle = 110.f;

	UPROPERTY(ReplicatedUsing="OnRep_LidOpen")
	bool bLidOpen;

	/* Required Tags in order to open the chest */
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer RequiredTags;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_LidOpen();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	bool GetLidOpen() const;
};
