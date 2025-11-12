// Galla

#include "AI/Services/GP_FindEnemyService.h"
#include "AI/GP_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Components/GP_AIPerceptionComponent.h"


UGP_FindEnemyService::UGP_FindEnemyService()
{
	NodeName = "Find Enemy";
}

void UGP_FindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		if (Controller)
		{
			const auto PerceptionComponent = Controller->GetComponentByClass<UGP_AIPerceptionComponent>();
			if (PerceptionComponent)
			{
				Blackboard->SetValueAsObject(TargetActorKey.SelectedKeyName, PerceptionComponent->GetTargetEnemy());
			}
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}