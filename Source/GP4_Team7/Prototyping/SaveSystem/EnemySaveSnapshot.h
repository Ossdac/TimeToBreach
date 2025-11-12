#pragma once
#include "CoreMinimal.h"
#include "EnemySaveSnapshot.generated.h"

USTRUCT(BlueprintType)
struct FEnemySaveSnapshot
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Category="SaveGame|Snapshot", SaveGame)
	FVector Location = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly, Category="SaveGame|Snapshot", SaveGame)
	FRotator Rotation = FRotator::ZeroRotator;
	
	UPROPERTY(BlueprintReadOnly, Category="SaveGame|Snapshot", SaveGame)
	float Health = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category="SaveGame|Snapshot", SaveGame)
	bool IsEmpty = true;
};
