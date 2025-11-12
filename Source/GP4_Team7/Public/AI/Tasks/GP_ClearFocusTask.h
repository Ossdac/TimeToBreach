// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP_ClearFocusTask.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_ClearFocusTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UGP_ClearFocusTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
