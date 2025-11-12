// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySaveStateSnapshotComponent.h"

#include "AI/GP_AICharacter.h"
#include "LogSaveCategory.h"
#include "SaveGameInstanceSubsystem.h"
#include "Components/GP_HealthComponent.h"

// Sets default values for this component's properties
UEnemySaveStateSnapshotComponent::UEnemySaveStateSnapshotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEnemySaveStateSnapshotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (const UWorld* World = GetWorld(); !IsValid(World) || !World->IsGameWorld()) return;
	if (AActor* Owner = GetOwner(); IsValid(Owner))
	{
		EnemyID = URegisterSaveComponent::CalculateIdentifier(Owner);
	}

	if (UGP_HealthComponent* HealthComponent = GetOwner()->FindComponentByClass<UGP_HealthComponent>();
		IsValid(HealthComponent))
	{
		HealthComponent->OnDeath.AddDynamic(this, &UEnemySaveStateSnapshotComponent::SaveDeathState);
	}
	else
	{
		UE_LOG(LogSavable,
			Error,
			TEXT("UEnemySaveStateSnapshotComponent: Owner does not have a valid HealthComponent to bind OnDeath event"));
	}
}

void UEnemySaveStateSnapshotComponent::SaveDeathState()
{
	if (const UWorld* World = GetWorld(); IsValid(World))
	{
		if (!World->IsGameWorld()) return;
		if (AActor* Owner = GetOwner(); IsValid(Owner))
		{
			if (Owner->GetClass()->ImplementsInterface(USavable::StaticClass()))
			{
				if (USaveGameInstanceSubsystem* SaveGameInstance = World->GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>();
					SaveGameInstance && IsValid(SaveGameInstance))
				{
					SaveGameInstance->UnregisterSavable(Owner);
					if (UMyGameSave* CurrentSaveGame = SaveGameInstance->CurrentSaveGame;
						IsValid(CurrentSaveGame))
					{
						CurrentSaveGame->SaveDeadEnemySnapshot(Cast<AGP_AICharacter>(Owner), EnemyID);
					}
					else
					{
						UE_LOG(LogSavable, Warning, TEXT("UEnemySaveStateSnapshotComponent: CurrentSaveGame is not valid"));
					}
				}
				else
				{
					UE_LOG(LogSavable, Warning, TEXT("UEnemySaveStateSnapshotComponent: Owner does not implement ISavable interface"));
				}
			}
		}
		else
		{
			UE_LOG(LogSavable, Warning, TEXT("UEnemySaveStateSnapshotComponent: Owner is not valid"));
		}
	}
}

