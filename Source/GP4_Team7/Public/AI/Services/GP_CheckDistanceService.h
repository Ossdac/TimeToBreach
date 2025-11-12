// Galla

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GP_CheckDistanceService.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_CheckDistanceService : public UBTService
{
	GENERATED_BODY()

public:
	UGP_CheckDistanceService();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RequiredDistance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AcceptableRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector IsCloseEnoughKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
