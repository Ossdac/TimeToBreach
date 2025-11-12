// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameInstanceSubsystem.generated.h"

class UMyGameSave;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGameSaved, const FString&, SaveName, const int32, SaveSlot, bool,
                                               bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGameLoaded,
	const FString&, SaveName,
	const int32, SaveSlot,
	bool, bSuccess);

/**
 * 
 */
UCLASS()
class GP4_TEAM7_API USaveGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Transient)
	TArray<AActor*> RegisteredSavables;

public:
	UPROPERTY(BlueprintAssignable, Category="SaveGame")
	FOnGameSaved GameSaved;

	UPROPERTY(BlueprintAssignable, Category="SaveGame")
	FOnGameLoaded GameLoaded;
	
	UPROPERTY(VisibleAnywhere, Category="SaveGame", Transient, BlueprintReadOnly)
	UMyGameSave* CurrentSaveGame;

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	UMyGameSave* CreateNewSaveGame();

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	bool SaveGame(const FString& SlotName, const int32 UserIndex);

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	bool LoadSaveGame(const FString& SlotName, int32 UserIndex, bool bSkipLevelLoad);

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	UMyGameSave* LoadOrCreateSaveGame(const FString& SlotName, int32 UserIndex, bool bSkipLevelLoad);
	
	void RegisterSavable(AActor* Savable);
	void UnregisterSavable(AActor* Savable);
};
