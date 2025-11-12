// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Team7AttributeSet.h"

#include "Net/UnrealNetwork.h"

UTeam7AttributeSet::UTeam7AttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
}

void UTeam7AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTeam7AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTeam7AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UTeam7AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTeam7AttributeSet, Health, OldHealth);
}

void UTeam7AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTeam7AttributeSet, MaxHealth, OldMaxHealth);
}
