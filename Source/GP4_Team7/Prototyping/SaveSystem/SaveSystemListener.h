// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SaveSystemListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadGame);

/**
 * 
 */
UCLASS()
class GP4_TEAM7_API USaveSystemListener : public UWorldSubsystem
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable, Category="SaveGame")
	FOnLoadGame OnLoadGame;

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void BroadcastLoadGame();
};
