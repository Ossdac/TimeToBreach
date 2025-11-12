// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySaveSnapshot.h"
#include "RoomDiscoveredSnapshot.h"
#include "GameFramework/SaveGame.h"
#include "MyGameSave.generated.h"

class AGP_AICharacter;
/**
 * 
 */
UCLASS()
class GP4_TEAM7_API UMyGameSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="SaveGame", BlueprintReadWrite, SaveGame)
	FString LevelName;
	
	UPROPERTY(EditAnywhere, Category="SaveGame", BlueprintReadWrite, SaveGame)
	FVector PlayerLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category="SaveGame", BlueprintReadWrite, SaveGame)
	TMap<int32, FRoomDiscoveredSnapshot> RoomSnapshots;

	UPROPERTY(EditAnywhere, Category="SaveGame", BlueprintReadWrite, SaveGame)
	TMap<int64, int32> CollectedItems;

	UPROPERTY(EditAnywhere, Category="SaveGame", BlueprintReadWrite, SaveGame)
	TMap<int64, FEnemySaveSnapshot> EnemySnapshots;

	UPROPERTY(EditAnywhere, Category="SaveGame", BlueprintReadWrite, SaveGame)
	TSet<int64> PickedUpItems;

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void SaveEnemySnapshot(AGP_AICharacter* Actor, int64 EnemyID);
	
	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void SaveDeadEnemySnapshot(AGP_AICharacter* Actor, int64 EnemyID);

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void LoadEnemySnapshot(AGP_AICharacter* Actor, int64 EnemyID);

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	FEnemySaveSnapshot GetEnemySnapshot(int64 EnemyID);

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	bool SavePickedUpItemState(int64 NameHash);

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void LoadPickedUpItemState(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void SaveToRoomSnapshot(int32 RoomID, const FRoomDiscoveredSnapshot& Snapshot);

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	FRoomDiscoveredSnapshot LoadFromRoomSnapshot(int32 RoomID);

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	bool IsCurrentLevelFromWorld(const UWorld* World) const;

	void CollectItem(int64 ItemID, int32 Count);
};
