// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameSave.h"
#include "AI/GP_AICharacter.h"
#include "Components/GP_HealthComponent.h"
#include "LogSaveCategory.h"
#include "Kismet/GameplayStatics.h"

void UMyGameSave::SaveEnemySnapshot(AGP_AICharacter* Actor, const int64 EnemyID)
{
	if (!IsValid(Actor)) return;
	FEnemySaveSnapshot Snapshot;
	Snapshot.Location = Actor->GetActorLocation();
	Snapshot.Rotation = Actor->GetActorRotation();
	if (const UGP_HealthComponent* HealthComponent = Actor->FindComponentByClass<UGP_HealthComponent>())
	{
		Snapshot.Health = HealthComponent->GetCurrentHealth();
	}

	if (EnemySnapshots.Contains(EnemyID))
	{
		EnemySnapshots[EnemyID] = Snapshot;
		return;
	}
	
	EnemySnapshots.Add(EnemyID, Snapshot);
	
}

void UMyGameSave::SaveDeadEnemySnapshot(AGP_AICharacter* Actor, int64 EnemyID)
{
	if (!IsValid(Actor))
	{
		UE_LOG(LogSavable, Warning, TEXT("SaveDeadEnemySnapshot: Actor is not valid"));
		return;
	}
	FEnemySaveSnapshot Snapshot;
	Snapshot.Location = Actor->GetActorLocation();
	Snapshot.Rotation = Actor->GetActorRotation();
	Snapshot.Health = 0.0f;

	if (EnemySnapshots.Contains(EnemyID))
	{
		EnemySnapshots[EnemyID] = Snapshot;
		UE_LOG(LogSavable, Display, TEXT("Updated dead enemy snapshot for ID: %lld"), EnemyID);
		return;
	}

	EnemySnapshots.Add(EnemyID, Snapshot);
	UE_LOG(LogSavable, Display, TEXT("Saved dead enemy snapshot for ID: %lld"), EnemyID);
}

void UMyGameSave::LoadEnemySnapshot(AGP_AICharacter* Actor, const int64 EnemyID)
{
	if (!IsValid(Actor))
	{
		UE_LOG(LogSavable, Warning, TEXT("LoadEnemySnapshot: Actor is not valid with ID: %lld"), EnemyID);
		return;
	}
	
	if (EnemySnapshots.Contains(EnemyID))
	{
		const FEnemySaveSnapshot Snapshot = EnemySnapshots[EnemyID];
		Actor->SetActorLocation(Snapshot.Location,
			false,
			nullptr,
			ETeleportType::TeleportPhysics);

		Actor->SetActorRotation(Snapshot.Rotation,
			ETeleportType::TeleportPhysics);

		if (Snapshot.Health <= 0.0f)
		{
			Actor->Destroy();
			UE_LOG(LogSavable, Display, TEXT("Destroyed enemy with ID: %lld"), EnemyID);
			return;
		}
		if (Snapshot.Health == 100.0f) return;		
		if (UGP_HealthComponent* HealthComponent = Actor->FindComponentByClass<UGP_HealthComponent>(); IsValid(HealthComponent))
		{
			HealthComponent->SetCurrentHealth(Snapshot.Health);
		}
	}
}

FEnemySaveSnapshot UMyGameSave::GetEnemySnapshot(const int64 EnemyID)
{
	if (EnemySnapshots.Contains(EnemyID))
	{
		return EnemySnapshots[EnemyID];
	}
	
	return FEnemySaveSnapshot();
}

bool UMyGameSave::SavePickedUpItemState(const int64 NameHash)
{
	UE_LOG(LogSavable, Display, TEXT("Saved pickup state for hash: %lld"), NameHash);
	return PickedUpItems.Add(NameHash).IsValidId();
}

void UMyGameSave::LoadPickedUpItemState(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		UE_LOG(LogSavable, Warning, TEXT("LoadPickedUpItemState: Actor is not valid"));
		return;
	}
	const int64 NameHash = URegisterSaveComponent::CalculateIdentifier(Actor);
	UE_LOG(LogSavable, Display, TEXT("Checking pickup state for hash: %lld"), NameHash);
	if (PickedUpItems.Contains(NameHash))
	{
		UE_LOG(LogSavable, Display, TEXT("Destroying picked up item with hash: %lld"), NameHash);
		Actor->Destroy();
	}
}

void UMyGameSave::SaveToRoomSnapshot(const int32 RoomID, const FRoomDiscoveredSnapshot& Snapshot)
{
	if (RoomSnapshots.Contains(RoomID))
	{
		RoomSnapshots[RoomID] = Snapshot;
		return;
	}
	
	RoomSnapshots.Add(RoomID, Snapshot);
}

FRoomDiscoveredSnapshot UMyGameSave::LoadFromRoomSnapshot(int32 RoomID)
{
	if (RoomSnapshots.Contains(RoomID))
	{
		return RoomSnapshots[RoomID];
	}
	
	return FRoomDiscoveredSnapshot();
}

bool UMyGameSave::IsCurrentLevelFromWorld(const UWorld* World) const
{
	if (IsValid(World))
	{
		const FString WorldLevelName = UGameplayStatics::GetCurrentLevelName(World, true);
		return WorldLevelName.Compare(LevelName) == 0;
	}
	return false;
}

void UMyGameSave::CollectItem(const int64 ItemID, const int32 Count)
{
	if (CollectedItems.Contains(ItemID))
	{
		CollectedItems[ItemID] += Count;
		return;
	}
	
	CollectedItems.Add(ItemID, Count);
}
