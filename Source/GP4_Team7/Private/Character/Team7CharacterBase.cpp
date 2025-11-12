// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Team7CharacterBase.h"


ATeam7CharacterBase::ATeam7CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent* ATeam7CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATeam7CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}