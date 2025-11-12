// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utils/GP_AttackTokenSubsystem.h"
#include "AIController.h"
#include "AI/GP_AIController.h"

void UGP_AttackTokenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	check(MaxConcurrentAttackers > 0);

	SetTokenAmount(MaxConcurrentAttackers);
}

bool UGP_AttackTokenSubsystem::RequestToken(AAIController* Requestor, int32 RequestedTokenAmount)
{
	if (!Requestor || RequestedTokenAmount < 0) return false;

	if (ActiveHolders.Num() == MaxConcurrentAttackers)
		return false;
	// just for one holder
	/*if (ActiveHolder.IsValid())
		return false;*/

	if (CurrentTokenAmount >= RequestedTokenAmount)
	{
		SetTokenAmount(CurrentTokenAmount - RequestedTokenAmount);
		AGP_AIController* EnemyController = Cast<AGP_AIController>(Requestor);
		if (EnemyController)
		{
			EnemyController->SetReservedAttackTokens(EnemyController->GetReservedAttackTokens() + RequestedTokenAmount);
		}
		//ActiveHolder = Requestor; // just for one holder
		if (ActiveHolders.Num() < MaxConcurrentAttackers)
		{
			ActiveHolders.Add(Requestor);
			return true;
		}
	}

	return false;
}

void UGP_AttackTokenSubsystem::ReleaseToken(AAIController* Holder, int32 ReturnedTokenAmount)
{
	if (!Holder || ReturnedTokenAmount < 0) return;

	if(!ActiveHolders.Contains(Holder))
	{
		return;
	}

	AGP_AIController* EnemyController = Cast<AGP_AIController>(Holder);
	if (EnemyController)
	{
		SetTokenAmount(CurrentTokenAmount + ReturnedTokenAmount);
		EnemyController->SetReservedAttackTokens(EnemyController->GetReservedAttackTokens() - ReturnedTokenAmount);
		//ActiveHolder.Reset(); // just for one holder

		int32 index = ActiveHolders.IndexOfByKey(Holder);
		ActiveHolders.RemoveAt(index);
	}
}

void UGP_AttackTokenSubsystem::SetTokenAmount(int32 NewAmount)
{
	const auto NextTokenAmount = FMath::Clamp(NewAmount, 0.0f, MaxConcurrentAttackers);

	CurrentTokenAmount = NextTokenAmount;
}
