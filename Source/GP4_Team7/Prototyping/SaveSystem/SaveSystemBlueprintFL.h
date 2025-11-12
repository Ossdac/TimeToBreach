// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveSystemBlueprintFL.generated.h"

class USaveGameInstanceSubsystem;
class UMyGameSave;
/**
 * 
 */
UCLASS()
class GP4_TEAM7_API USaveSystemBlueprintFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="SaveGame", meta=(WorldContext="WorldContextObject"))
	static void SavePlayerLocation(UObject* WorldContextObject, FVector Location);

	UFUNCTION(BlueprintCallable, Category="SaveGame", meta=(WorldContext="WorldContextObject"))
	static void SaveGame(UObject* WorldContextObject, FString SlotName, int32 UserIndex);

	UFUNCTION(BlueprintCallable, Category="SaveGame", meta=(WorldContext="WorldContextObject"))
	static bool CreateFreshSaveGame(UObject* WorldContextObject, FString SlotName, int32 UserIndex);

	UFUNCTION(BlueprintCallable, Category="SaveGame", meta=(WorldContext="WorldContextObject"))
	static UMyGameSave* LoadOrCreateSaveGame(UObject* WorldContextObject, FString SlotName, int32 UserIndex);

	UFUNCTION(BlueprintCallable, Category="SaveGame", meta=(WorldContext="WorldContextObject"))
	static USaveGameInstanceSubsystem* GetSaveGameInstanceSubsystem(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="SaveGame", meta=(WorldContext="WorldContextObject") )
	static UMyGameSave* GetCurrentSaveGame(UObject* WorldContextObject);
};
