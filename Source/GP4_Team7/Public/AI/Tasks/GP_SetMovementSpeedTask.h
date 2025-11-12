// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/GP_AICharacter.h" // should change it
#include "BehaviorTree/BTTaskNode.h"
#include "GP_SetMovementSpeedTask.generated.h"


UCLASS()
class GP4_TEAM7_API UGP_SetMovementSpeedTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UGP_SetMovementSpeedTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TEnumAsByte<ESpeedState> SpeedState;
	
};
