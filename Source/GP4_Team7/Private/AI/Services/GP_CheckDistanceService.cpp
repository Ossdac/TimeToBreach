// Galla

#include "AI/Services/GP_CheckDistanceService.h"
#include "AI/GP_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGP_CheckDistanceService::UGP_CheckDistanceService()
{
	NodeName = "Check Distance";
}

void UGP_CheckDistanceService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		Blackboard->SetValueAsBool(IsCloseEnoughKey.SelectedKeyName, false);
		return;
	}

	const float Distance = FVector::Dist(Target->GetActorLocation(), AIPawn->GetActorLocation());

	bool IsCloseEnough = UKismetMathLibrary::InRange_FloatFloat(Distance, 0.f, RequiredDistance + AcceptableRadius, false, true);

	Blackboard->SetValueAsBool(IsCloseEnoughKey.SelectedKeyName, IsCloseEnough);
}
