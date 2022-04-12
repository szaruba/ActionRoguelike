#pragma once

#include "Engine/DataTable.h"
#include "FSMonsterInfoRow.generated.h"

USTRUCT(BlueprintType)
struct FSMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	FSMonsterInfoRow()
	{
		Weight = 1.0f;
		CreditReward = 80.f;
		SpawnCost = 20.f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPrimaryDataAsset* MonsterData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CreditReward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;
};
