// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSaveLoaderComponent.h"

#include "RegisterSaveComponent.h"
#include "LogSaveCategory.h"
#include "Savable.h"
#include "SaveGameInstanceSubsystem.h"

// Sets default values for this component's properties
UPickupSaveLoaderComponent::UPickupSaveLoaderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPickupSaveLoaderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (const UWorld* World = GetWorld(); IsValid(World))
	{
		if (!World->IsGameWorld()) return;
		if (USaveGameInstanceSubsystem* SaveGameInstance = World->GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>();
			SaveGameInstance && IsValid(SaveGameInstance))
		{
			if (AActor* Owner = GetOwner(); IsValid(Owner))
			{
				if (Owner->GetClass()->ImplementsInterface(USavable::StaticClass()))
				{
					SaveGameInstance->RegisterSavable(Owner);
					if (UMyGameSave* CurrentSaveGame = SaveGameInstance->CurrentSaveGame;
						IsValid(CurrentSaveGame))
					{						
						ISavable::Execute_LoadFromSaveGame(Owner, CurrentSaveGame, !CurrentSaveGame->IsCurrentLevelFromWorld(World));
					}
					else
					{
						UE_LOG(LogSavable, Warning, TEXT("URegisterSaveComponent: CurrentSaveGame is not valid"));
					}
				}
				else
				{
					UE_LOG(LogSavable, Warning, TEXT("URegisterSaveComponent: Owner does not implement Savable interface"));
				}
			}
			else
			{
				UE_LOG(LogSavable, Warning, TEXT("URegisterSaveComponent: Owner is not valid"));
			}
		}
		else
		{
			UE_LOG(LogSavable, Warning, TEXT("URegisterSaveComponent: SaveGameInstance is not valid"));
		}
	}
}

bool UPickupSaveLoaderComponent::SavePickupState()
{
	if (const UWorld* World = GetWorld(); IsValid(World))
	{
		if (!World->IsGameWorld()) return false;
		if (const USaveGameInstanceSubsystem* SaveGameInstance = World->GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>();
			SaveGameInstance && IsValid(SaveGameInstance))
		{
			if (AActor* Owner = GetOwner(); IsValid(Owner))
			{
				if (Owner->GetClass()->ImplementsInterface(USavable::StaticClass()))
				{
					if (UMyGameSave* CurrentSaveGame = SaveGameInstance->CurrentSaveGame;
						IsValid(CurrentSaveGame))
					{
						const int64 NameHash = URegisterSaveComponent::CalculateIdentifier(Owner);
						return CurrentSaveGame->SavePickedUpItemState(NameHash);
					}
					UE_LOG(LogSavable, Warning, TEXT("UPickupSaveLoaderComponent: CurrentSaveGame is not valid"));
				}
				else
				{
					UE_LOG(LogSavable, Warning, TEXT("UPickupSaveLoaderComponent: Owner does not implement Savable interface"));
				}
			}
			else
			{
				UE_LOG(LogSavable, Warning, TEXT("UPickupSaveLoaderComponent: Owner is not valid"));
			}
		}
		else
		{
			UE_LOG(LogSavable, Warning, TEXT("UPickupSaveLoaderComponent: SaveGameInstance is not valid"));
		}
	}

	return false;
}

