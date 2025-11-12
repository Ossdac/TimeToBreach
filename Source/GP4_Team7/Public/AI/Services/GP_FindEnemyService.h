// Galla

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GP_FindEnemyService.generated.h"


UCLASS()
class GP4_TEAM7_API UGP_FindEnemyService : public UBTService
{
	GENERATED_BODY()

public:
	UGP_FindEnemyService();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetActorKey;	

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
