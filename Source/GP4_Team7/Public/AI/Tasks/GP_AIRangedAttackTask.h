// Galla

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP_AIRangedAttackTask.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_AIRangedAttackTask : public UBTTaskNode
{
	GENERATED_BODY()

public: 

	UGP_AIRangedAttackTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:

	UFUNCTION()
	void HandleRangedAttackFinished();

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;

	UPROPERTY()
	AAIController* CachedController = nullptr;
};
