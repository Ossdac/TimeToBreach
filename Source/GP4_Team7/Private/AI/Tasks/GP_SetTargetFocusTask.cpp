// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/GP_SetTargetFocusTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(GP_SetTargetFocusTaskLog, All, All);

UGP_SetTargetFocusTask::UGP_SetTargetFocusTask()
{
	NodeName = "Set Target Focus";
}

EBTNodeResult::Type UGP_SetTargetFocusTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Controller || !Blackboard)
	{
		UE_LOG(GP_SetTargetFocusTaskLog, Display, TEXT("Not Valid"));
		return EBTNodeResult::Failed;
	}
	const auto bHasTarget = Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName);

	if (bHasTarget)
	{
		Controller->SetFocus(Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName)));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}