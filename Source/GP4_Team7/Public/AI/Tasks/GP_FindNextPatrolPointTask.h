// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP_FindNextPatrolPointTask.generated.h"


UCLASS()
class GP4_TEAM7_API UGP_FindNextPatrolPointTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UGP_FindNextPatrolPointTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bChooseRandomPoint = false;
	
};
