// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/GP_FindNextPatrolPointTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/GP_AICharacter.h"
#include "AI/Utils/GP_PatrolRouteActor.h"

DEFINE_LOG_CATEGORY_STATIC(GP_FindNextPatrolPointTaskLog, All, All);

UGP_FindNextPatrolPointTask::UGP_FindNextPatrolPointTask()
{
	NodeName = "Find Next Patrol Point Task";
}

EBTNodeResult::Type UGP_FindNextPatrolPointTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto Owner = Cast<AGP_AICharacter>(Controller->GetPawn());

	if (!Controller || !Blackboard || !Owner)
	{
		UE_LOG(GP_FindNextPatrolPointTaskLog, Display, TEXT("Not Valid"));
		return EBTNodeResult::Failed;
	}

	if (!IsValid(Owner->PatrolRoute))
	{
		return EBTNodeResult::Failed;
	}

	//if (IsValid(Owner->PatrolRoute))
	//{
	//	FVector Destination = Owner->PatrolRoute->GetSplinePointAsWorldPosition();

	//	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, Destination);
	//	//Controller->MoveToActor()
	//	Owner->PatrolRoute->IncrementPatrolPath();
	//}
	//else
	//{
	//	return EBTNodeResult::Failed;
	//}

	//return EBTNodeResult::Succeeded;

	FVector Destination = FVector::ZeroVector;

	if (bChooseRandomPoint)
	{
		Destination = Owner->PatrolRoute->GetRandomSplinePointAsWorldPosition();
	}
	else
	{
		Destination = Owner->PatrolRoute->GetSplinePointAsWorldPosition();
		Owner->PatrolRoute->IncrementPatrolPath();
	}

	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, Destination);
	//Controller->MoveToActor()

	return EBTNodeResult::Succeeded;
}
