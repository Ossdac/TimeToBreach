// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/GP_EQSContext_TargetLocation.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void UGP_EQSContext_TargetLocation::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
	if (!Blackboard) return;
    FVector Loc = Blackboard->GetValueAsVector(TargetLocationKeyName);
	UEnvQueryItemType_Point::SetContextHelper(ContextData, Loc);
}
