// Fill out your copyright notice in the Description page of Project Settings.


#include "RegisterSaveComponent.h"
#include "LogSaveCategory.h"
#include "Savable.h"
#include "SaveGameInstanceSubsystem.h"

// Sets default values for this component's properties
URegisterSaveComponent::URegisterSaveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URegisterSaveComponent::BeginPlay()
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
						CachedIdentifier = CalculateIdentifier(Owner);
						CachedName = Owner->GetName();
						
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

void URegisterSaveComponent::BeginDestroy()
{
	Super::BeginDestroy();
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
					SaveGameInstance->UnregisterSavable(Owner);
					if (UMyGameSave* CurrentSaveGame = SaveGameInstance->CurrentSaveGame;
						IsValid(CurrentSaveGame))
					{
						ISavable::Execute_SaveToSaveGame(Owner, CurrentSaveGame);
					}
				}
			}
			else
			{
				UE_LOG(LogSavable,
					Warning,
					TEXT("URegisterSaveComponent: Owner (%ls) is not valid"),
					*CachedName);
			}
		}
		else
		{
			UE_LOG(LogSavable,
				Warning,
				TEXT("URegisterSaveComponent: SaveGameInstance for actor (%ls) is not valid"),
				*CachedName);
		}
	}
}

int32 URegisterSaveComponent::GetIdentifier() const
{
	return CalculateIdentifier(GetOwner());
}

int32 URegisterSaveComponent::CalculateIdentifier(AActor* Actor)
{
	return FCrc::StrCrc32(*Actor->GetName());
}

