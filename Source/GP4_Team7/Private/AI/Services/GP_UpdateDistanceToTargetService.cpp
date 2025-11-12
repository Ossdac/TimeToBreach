// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/GP_UpdateDistanceToTargetService.h"
#include "AI/GP_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGP_UpdateDistanceToTargetService::UGP_UpdateDistanceToTargetService()
{
	NodeName = "Update Distance To Target";
}

void UGP_UpdateDistanceToTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto Controller = OwnerComp.GetAIOwner();

	if (!Blackboard || !Controller) return;

	const auto AIPawn = Controller->GetPawn();
	if (!AIPawn) return;

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target)
	{
		Blackboard->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, MAX_FLT);
		return;
	}

	const float Distance = FVector::Dist(Target->GetActorLocation(), AIPawn->GetActorLocation());;

	Blackboard->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, Distance);
}
