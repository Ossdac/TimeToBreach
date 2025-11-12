// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP_SetTargetFocusTask.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_SetTargetFocusTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UGP_SetTargetFocusTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetActorKey;
};
