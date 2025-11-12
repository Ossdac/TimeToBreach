// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP_AISecondMeleeAttackTask.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_AISecondMeleeAttackTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UGP_AISecondMeleeAttackTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:

	UFUNCTION()
	void HandleMeleeAttackFinished();

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;

	UPROPERTY()
	AAIController* CachedController = nullptr;
	
};
