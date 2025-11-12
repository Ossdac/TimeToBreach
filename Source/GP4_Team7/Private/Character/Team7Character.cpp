// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Team7Character.h"

#include "AbilitySystemComponent.h"
#include "Player/Team7PlayerState.h"

ATeam7Character::ATeam7Character()
{
	

	RegisterSaveComponent = CreateDefaultSubobject<URegisterSaveComponent>(TEXT("RegisterSaveComponent"));
}

void ATeam7Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void ATeam7Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void ATeam7Character::InitAbilityActorInfo()
{
	ATeam7PlayerState* Team7PlayerState = GetPlayerState<ATeam7PlayerState>();
	check(Team7PlayerState);
	Team7PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(Team7PlayerState, this);
	AbilitySystemComponent = Team7PlayerState->GetAbilitySystemComponent();
	AttributeSet = Team7PlayerState->GetAttributeSet();
}
