// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/GP_AISecondMeleeAttackTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/GP_AICharacter.h"
#include "AI/Utils/GP_AttackTokenSubsystem.h"
#include "AI/Interfaces/GP_AIMeleeAttackInterface.h"

#include "AI/Components/GP_HumanoidMeleeAttackComponent.h"

DEFINE_LOG_CATEGORY_STATIC(GP_AISecondMeleeAttackTaskLog, All, All);

UGP_AISecondMeleeAttackTask::UGP_AISecondMeleeAttackTask()
{
	NodeName = "AI Second Melee Attack";
}

EBTNodeResult::Type UGP_AISecondMeleeAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Controller || !Blackboard)
	{
		UE_LOG(GP_AISecondMeleeAttackTaskLog, Display, TEXT("!Controller || !Blackboard"));
		return EBTNodeResult::Failed;
	}

	UGP_AttackTokenSubsystem* AttackTokenSubsystem = GetWorld()->GetSubsystem<UGP_AttackTokenSubsystem>();
	if (!AttackTokenSubsystem)
	{
		UE_LOG(GP_AISecondMeleeAttackTaskLog, Error, TEXT("No AttackTokenSubsystem in world"));
		return EBTNodeResult::Failed;
	}

	if (!AttackTokenSubsystem->CanRequestToken())
	{
		UE_LOG(GP_AISecondMeleeAttackTaskLog, Error, TEXT("Request Token is denied for %s"), *Controller->GetName());
		return EBTNodeResult::Failed;
	}

	CachedOwnerComp = &OwnerComp;
	CachedController = OwnerComp.GetAIOwner();

	const auto Owner = Cast<AGP_AICharacter>(Controller->GetPawn());
	if (!Owner)
	{
		UE_LOG(GP_AISecondMeleeAttackTaskLog, Display, TEXT("!Owner"));
		return EBTNodeResult::Failed;
	}

	const auto HasTarget = Blackboard && Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName);
	if (HasTarget && Controller->GetPawn()->Implements<UGP_AIMeleeAttackInterface>())
	{
		UE_LOG(GP_AISecondMeleeAttackTaskLog, Display, TEXT("HasTarget"));
		/*IGP_AIMeleeAttackInterface* AttackOWner = Cast<IGP_AIMeleeAttackInterface>(Controller->GetPawn());
		if (!AttackOWner)
		{
			UE_LOG(GP_AISecondMeleeAttackTaskLog, Error, TEXT("No AttackOWner in world"));
			return EBTNodeResult::Failed;
		}*/
		UGP_HumanoidMeleeAttackComponent* AttackComponent = Controller->GetPawn()->FindComponentByClass<UGP_HumanoidMeleeAttackComponent>();
		if (!AttackComponent)
		{
			UE_LOG(GP_AISecondMeleeAttackTaskLog, Error, TEXT("No AttackComponent in world"));
			return EBTNodeResult::Failed;
		}

		if (!IGP_AIMeleeAttackInterface::Execute_CanPerformMeleeAttack(Controller->GetPawn()))
		{
			UE_LOG(GP_AISecondMeleeAttackTaskLog, Error, TEXT("Cannot Perform Second Melee Attack for %s"), *Owner->GetName());
			return EBTNodeResult::Failed;
		}

		if (AttackTokenSubsystem->RequestToken(Controller, 1))
		{
			auto& OnFinished = AttackComponent->GetAISecondMeleeAttackFinishedDelegate();
			OnFinished.RemoveAll(this);

			FScriptDelegate D;
			D.BindUFunction(this, FName("HandleMeleeAttackFinished"));
			OnFinished.Add(D);

			IGP_AIMeleeAttackInterface::Execute_PerformSecondMeleeAttack(Controller->GetPawn(), Owner, Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName)));
			//AttackOWner->PerformMeleeAttack(Owner, Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName)));
			UE_LOG(GP_AISecondMeleeAttackTaskLog, Display, TEXT("Attack: Controller = %s"), *Controller->GetName());


			return EBTNodeResult::InProgress;
		}
		else
		{
			UE_LOG(GP_AISecondMeleeAttackTaskLog, Error, TEXT("Request Token is denied for %s"), *Owner->GetName());
			return EBTNodeResult::Failed;
		}

	}
	else
	{
		UE_LOG(GP_AISecondMeleeAttackTaskLog, Display, TEXT("Has No Target"));
		return EBTNodeResult::Failed;
	}
}

EBTNodeResult::Type UGP_AISecondMeleeAttackTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller)
	{
		if (Controller->GetPawn() && Controller->GetPawn()->Implements<UGP_AIMeleeAttackInterface>())
		{
			UGP_AttackTokenSubsystem* AttackTokenSubsystem = GetWorld()->GetSubsystem<UGP_AttackTokenSubsystem>();

			if (AttackTokenSubsystem)
				AttackTokenSubsystem->ReleaseToken(Controller, 1);

			//IGP_AIMeleeAttackInterface* AttackOWner = Cast<IGP_AIMeleeAttackInterface>(Controller->GetPawn());
			UGP_HumanoidMeleeAttackComponent* AttackComponent = Controller->GetPawn()->FindComponentByClass<UGP_HumanoidMeleeAttackComponent>();

			IGP_AIMeleeAttackInterface::Execute_StopPerformSecondMeleeAttack(Controller->GetPawn());
			if (AttackComponent)
				AttackComponent->GetAISecondMeleeAttackFinishedDelegate().RemoveAll(this);
			//AttackOWner->StopPerformMeleeAttack();


			UE_LOG(GP_AISecondMeleeAttackTaskLog, Display, TEXT("AbortTask: Controller = %s"), *Controller->GetName());
		}
	}
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UGP_AISecondMeleeAttackTask::HandleMeleeAttackFinished()
{
	if (!CachedOwnerComp || !CachedController) return;

	if (UGP_AttackTokenSubsystem* AttackTokenSubsystem = GetWorld()->GetSubsystem<UGP_AttackTokenSubsystem>())
	{
		AttackTokenSubsystem->ReleaseToken(CachedController, 1);
	}

	UE_LOG(GP_AISecondMeleeAttackTaskLog, Display, TEXT("Second Melee attack finished for %s"), *CachedController->GetName());

	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

