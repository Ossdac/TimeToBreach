// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSave.h"
#include "UObject/Interface.h"
#include "Savable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class USavable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GP4_TEAM7_API ISavable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Save System")
	void LoadFromSaveGame(UMyGameSave* SaveGame, bool IsLevelChanging = false);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Save System")
	void SaveToSaveGame(UMyGameSave* SaveGame);
};
