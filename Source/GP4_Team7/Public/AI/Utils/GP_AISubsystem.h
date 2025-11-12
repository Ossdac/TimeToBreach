// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GP_AISubsystem.generated.h"

class AGP_AICharacter;
class AGP_AIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllAIForgotCharacterSignature);

UCLASS()
class GP4_TEAM7_API UGP_AISubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	TMap<AGP_AIController*, bool> AIListeners;
	
	void RegisterAIListener(AGP_AIController* Controller);
	void UnregisterAIListener(AGP_AIController* Controller);

	bool IsAllAIForgotCharacter();

	void SetIsCharacterForgotten(AGP_AIController* Controller, bool IsForgotten);

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAllAIForgotCharacterSignature OnAllAIForgotCharacter;
};
