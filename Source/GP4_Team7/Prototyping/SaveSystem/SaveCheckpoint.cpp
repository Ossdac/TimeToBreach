// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveCheckpoint.h"

#include "SaveSystemBlueprintFL.h"
#include "Character/Team7CharacterBase.h"
#include "LogSaveCategory.h"
#include "Components/BoxComponent.h"

// Sets default values
ASaveCheckpoint::ASaveCheckpoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAll"));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetupAttachment(RootComponent);
	
	OnActorBeginOverlap.AddDynamic(this, &ASaveCheckpoint::ActivateCheckpoint);
	OnActorEndOverlap.AddDynamic(this, &ASaveCheckpoint::DeactivateCheckpoint);
}

// Called when the game starts or when spawned
void ASaveCheckpoint::BeginPlay()
{
	Super::BeginPlay();
}

void ASaveCheckpoint::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCanActivateCheckpoint) return;
	if (CurrentCooldown < CooldownTimer)
	{
		CurrentCooldown += DeltaSeconds;
	}
	else
	{
		bCanActivateCheckpoint = true;
		CurrentCooldown = 0.0f;
	}
}

void ASaveCheckpoint::ActivateCheckpoint(AActor* ActorA, AActor* ActorB)
{
	if (!IsValid(ActorA) || !IsValid(ActorB)) return;
	if (!ActorB->IsA(ATeam7CharacterBase::StaticClass())) return;
	if (!bCanActivateCheckpoint)
	{
		UE_LOG(LogSavable, Display, TEXT("Checkpoint on cooldown"));
		return;
	}
	UE_LOG(LogSavable, Display, TEXT("Checkpoint Activated"));
	USaveSystemBlueprintFL::SaveGame(this, TEXT("test"), 0);
	
	// Toggles cooldown for checkpoint
	bCanActivateCheckpoint = false;
}

void ASaveCheckpoint::DeactivateCheckpoint(AActor* ActorA, AActor* ActorB)
{
	UE_LOG(LogSavable, Display, TEXT("Checkpoint Deactivated"));
}

