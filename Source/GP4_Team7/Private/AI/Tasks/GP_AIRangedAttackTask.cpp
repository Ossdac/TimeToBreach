// Galla

#include "AI/Tasks/GP_AIRangedAttackTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/GP_AICharacter.h"
#include "AI/Utils/GP_AttackTokenSubsystem.h"
#include "AI/Interfaces/GP_RangedAttackInterface.h"

#include "AI/Components/GP_AIRangedAttackComponent.h"

DEFINE_LOG_CATEGORY_STATIC(GP_AIRangedAttackTaskLog, All, All);

UGP_AIRangedAttackTask::UGP_AIRangedAttackTask()
{
	NodeName = "AI Ranged Attack";
}

EBTNodeResult::Type UGP_AIRangedAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Controller || !Blackboard)
	{
		//UE_LOG(GP_AIRangedAttackTaskLog, Display, TEXT("!Controller || !Blackboard"));
		return EBTNodeResult::Failed;
	}

	UGP_AttackTokenSubsystem* AttackTokenSubsystem = GetWorld()->GetSubsystem<UGP_AttackTokenSubsystem>();
	if (!AttackTokenSubsystem)
	{
		//UE_LOG(GP_AIRangedAttackTaskLog, Error, TEXT("No AttackTokenSubsystem in world"));
		return EBTNodeResult::Failed;
	}

	if (!AttackTokenSubsystem->CanRequestToken())
	{
		//UE_LOG(GP_AIRangedAttackTaskLog, Error, TEXT("Request Token is denied for %s"), *Controller->GetName());
		return EBTNodeResult::Failed;
	}

	CachedOwnerComp = &OwnerComp;
	CachedController = OwnerComp.GetAIOwner();

	const auto Owner = Cast<AGP_AICharacter>(Controller->GetPawn());
	if (!Owner)
	{
		//UE_LOG(GP_AIRangedAttackTaskLog, Display, TEXT("!Owner"));
		return EBTNodeResult::Failed;
	}

	const auto HasTarget = Blackboard && Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName);
	if (HasTarget && Controller->GetPawn()->Implements<UGP_RangedAttackInterface>())
	{
		//UE_LOG(GP_AIRangedAttackTaskLog, Display, TEXT("HasTarget"));
		/*IGP_RangedAttackInterface* AttackOWner = Cast<IGP_RangedAttackInterface>(Controller->GetPawn());
		if (!AttackOWner)
		{
			UE_LOG(GP_AIRangedAttackTaskLog, Error, TEXT("No AttackOWner in world"));
			return EBTNodeResult::Failed;
		}*/
		UGP_AIRangedAttackComponent* AttackComponent = Controller->GetPawn()->FindComponentByClass<UGP_AIRangedAttackComponent>();
		if (!AttackComponent)
		{
			//UE_LOG(GP_AIRangedAttackTaskLog, Error, TEXT("No AttackComponent in world"));
			return EBTNodeResult::Failed;
		}

		if (!IGP_RangedAttackInterface::Execute_CanPerformRangedAttack(Controller->GetPawn()))
		{
			//UE_LOG(GP_AIRangedAttackTaskLog, Error, TEXT("Cannot Perform Ranged Attack"));
			return EBTNodeResult::Failed;
		}

		if (AttackTokenSubsystem->RequestToken(Controller, 1))
		{
			auto& OnFinished = AttackComponent->GetAIRangedAttackFinishedDelegate();
			OnFinished.RemoveAll(this);

			FScriptDelegate D;
			D.BindUFunction(this, FName("HandleRangedAttackFinished"));
			OnFinished.Add(D);

			IGP_RangedAttackInterface::Execute_PerformRangedAttack(Controller->GetPawn(), Owner, Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName)));
			//AttackOWner->PerformRangedAttack(Owner, Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName)));
			//UE_LOG(GP_AIRangedAttackTaskLog, Display, TEXT("Attack: Controller = %s"), *Controller->GetName());


			return EBTNodeResult::InProgress;
		}
		else
		{
			//UE_LOG(GP_AIRangedAttackTaskLog, Error, TEXT("Request Token is denied for %s"), *Owner->GetName());
			return EBTNodeResult::Failed;
		}

	}
	else
	{
		//UE_LOG(GP_AIRangedAttackTaskLog, Display, TEXT("Has No Target"));
		return EBTNodeResult::Failed;
	}
}

EBTNodeResult::Type UGP_AIRangedAttackTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller)
	{
		if (Controller->GetPawn() && Controller->GetPawn()->Implements<UGP_RangedAttackInterface>())
		{
			UGP_AttackTokenSubsystem* AttackTokenSubsystem = GetWorld()->GetSubsystem<UGP_AttackTokenSubsystem>();
			
			if (AttackTokenSubsystem)
				AttackTokenSubsystem->ReleaseToken(Controller, 1);

			//IGP_RangedAttackInterface* AttackOWner = Cast<IGP_RangedAttackInterface>(Controller->GetPawn());
			UGP_AIRangedAttackComponent* AttackComponent = Controller->GetPawn()->FindComponentByClass<UGP_AIRangedAttackComponent>();
			
			IGP_RangedAttackInterface::Execute_StopPerformRangedAttack(Controller->GetPawn());
			if(AttackComponent)
				AttackComponent->GetAIRangedAttackFinishedDelegate().RemoveAll(this);
			//AttackOWner->StopPerformRangedAttack();
		}
	}
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UGP_AIRangedAttackTask::HandleRangedAttackFinished()
{
	if (!CachedOwnerComp || !CachedController) return;

	if (UGP_AttackTokenSubsystem* AttackTokenSubsystem = GetWorld()->GetSubsystem<UGP_AttackTokenSubsystem>())
	{
		AttackTokenSubsystem->ReleaseToken(CachedController, 1);
	}

	//UE_LOG(LogTemp, Display, TEXT("Ranged attack finished for %s"), *CachedController->GetName());

	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}
