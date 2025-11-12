// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GP_UpdateDistanceToTargetService.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_UpdateDistanceToTargetService : public UBTService
{
	GENERATED_BODY()

public:
	UGP_UpdateDistanceToTargetService();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector DistanceToTargetKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
