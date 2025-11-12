// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayGamemode.h"
#include "ObjectPool/ObjectPool.h"


AGameplayGamemode::AGameplayGamemode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL) { DefaultPawnClass = PlayerPawnBPClass.Class; }

	ProjectilePool = CreateDefaultSubobject<UObjectPool>("ProjectilePool");
}

void AGameplayGamemode::StartGlobalRewind()
{
	TRACE_BOOKMARK(TEXT("ARewindGameMode::StartGlobalRewind"));

	bIsGlobalRewinding = true;
	OnGlobalRewindStarted.Broadcast();
}

void AGameplayGamemode::RewindForSeconds(float Seconds)
{
	OnGlobalRewindSeconds.Broadcast(Seconds);
}

void AGameplayGamemode::StopGlobalRewind()
{
	TRACE_BOOKMARK(TEXT("ARewindGameMode::StopGlobalRewind"));

	bIsGlobalRewinding = false;
	OnGlobalRewindCompleted.Broadcast();
}

void AGameplayGamemode::StartGlobalFastForward()
{
	TRACE_BOOKMARK(TEXT("ARewindGameMode::StartGlobalFastForward"));

	bIsGlobalFastForwarding = true;
	OnGlobalFastForwardStarted.Broadcast();
}

void AGameplayGamemode::StopGlobalFastForward()
{
	TRACE_BOOKMARK(TEXT("ARewindGameMode::StopGlobalFastForward"));

	bIsGlobalFastForwarding = false;
	OnGlobalFastForwardCompleted.Broadcast();
}

void AGameplayGamemode::ToggleTimeScrub()
{
	TRACE_BOOKMARK(TEXT("ARewindGameMode::ToggleTimeScrub"));

	bIsGlobalTimeScrubbing = !bIsGlobalTimeScrubbing;
	if (bIsGlobalTimeScrubbing)
	{
		OnGlobalTimeScrubStarted.Broadcast();
	}
	else
	{
		OnGlobalTimeScrubCompleted.Broadcast();
	}
}

