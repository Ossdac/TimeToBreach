// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/GP_ClearFocusTask.h"
#include "AIController.h"

UGP_ClearFocusTask::UGP_ClearFocusTask()
{
	NodeName = "Clear Focus";
}

EBTNodeResult::Type UGP_ClearFocusTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();

	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		Controller->ClearFocus(EAIFocusPriority::Gameplay);
		return EBTNodeResult::Succeeded;
	}
}
