// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Team7PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM7_API ATeam7PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATeam7PlayerController();

protected:
	virtual void BeginPlay() override;
	
};
