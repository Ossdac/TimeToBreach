// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RewindableCharacter.h"


ARewindableCharacter::ARewindableCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	RewindComponent = CreateDefaultSubobject<URewindComponent>(TEXT("RewindComponent"));
	RewindComponent->bSnapshotAnimationVariables = true;
}

// Sets default values
ARewindableCharacter::ARewindableCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	RewindComponent = CreateDefaultSubobject<URewindComponent>(TEXT("RewindComponent"));
	RewindComponent->bSnapshotAnimationVariables = true;
}


