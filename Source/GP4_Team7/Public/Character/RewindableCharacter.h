// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gameplay/Time/TimeReversal/RewindComponent.h"
#include "RewindableCharacter.generated.h"

UCLASS()
class GP4_TEAM7_API ARewindableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARewindableCharacter();
	ARewindableCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URewindComponent* RewindComponent;

	
};
