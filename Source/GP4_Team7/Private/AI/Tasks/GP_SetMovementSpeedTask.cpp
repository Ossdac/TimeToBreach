// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/GP_SetMovementSpeedTask.h"
#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(GP_SetMovementSpeedTaskLog, All, All);

UGP_SetMovementSpeedTask::UGP_SetMovementSpeedTask()
{
	NodeName = "Set Movement Speed";
}

EBTNodeResult::Type UGP_SetMovementSpeedTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Owner = Cast<AGP_AICharacter>(Controller->GetPawn());

	if (!Controller || !Owner)
	{
		UE_LOG(GP_SetMovementSpeedTaskLog, Display, TEXT("Not Valid"));
		return EBTNodeResult::Failed;
	}

	Owner->SetMovementSpeed(SpeedState);

	return EBTNodeResult::Succeeded;
}
