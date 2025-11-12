// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Team7PlayerState.h"

#include "AbilitySystem/Team7AbilitySystemComponent.h"
#include "AbilitySystem/Team7AttributeSet.h"

ATeam7PlayerState::ATeam7PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTeam7AbilitySystemComponent>("AbilitySystemComponent");

	AttributeSet = CreateDefaultSubobject<UTeam7AttributeSet>("AttributeSet");

	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* ATeam7PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
