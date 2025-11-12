// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystemBlueprintFL.h"
#include "LogSaveCategory.h"
#include "MyGameSave.h"
#include "SaveGameInstanceSubsystem.h"

void USaveSystemBlueprintFL::SavePlayerLocation(UObject* WorldContextObject, FVector Location)
{
	if (!IsValid(WorldContextObject)) return;
	if (const UWorld* World = WorldContextObject->GetWorld(); IsValid(World))
	{
		if (const USaveGameInstanceSubsystem* SaveGameInstance = World->GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>(); IsValid(SaveGameInstance))
		{
			if (UMyGameSave* SaveGame = SaveGameInstance->CurrentSaveGame;
				IsValid(SaveGame))
			{
				SaveGame->PlayerLocation = Location;
			}
		}
	}
}

void USaveSystemBlueprintFL::SaveGame(UObject* WorldContextObject, FString SlotName, int32 UserIndex)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogSavable, Warning, TEXT("USaveSystemBlueprintFL::SaveGame: WorldContextObject is not valid"));
		return;
	}
	if (const UWorld* World = WorldContextObject->GetWorld(); IsValid(World))
	{
		if (USaveGameInstanceSubsystem* SaveGameInstance = World->GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>();
			IsValid(SaveGameInstance))
		{
			SaveGameInstance->SaveGame(SlotName, UserIndex);
		}
		else
		{
			UE_LOG(LogSavable, Warning, TEXT("USaveSystemBlueprintFL::SaveGame: GameInstance is not a SaveGameInstance"));
		}
	}
	else
	{
		UE_LOG(LogSavable, Warning, TEXT("USaveSystemBlueprintFL::SaveGame: WorldContextObject has no valid World"));
	}
}

bool USaveSystemBlueprintFL::CreateFreshSaveGame(UObject* WorldContextObject, FString SlotName, int32 UserIndex)
{
	if (!IsValid(WorldContextObject)) return false;
	if (const UWorld* World = WorldContextObject->GetWorld(); IsValid(World))
	{
		if (USaveGameInstanceSubsystem* SaveGameInstance = World->GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>();
			IsValid(SaveGameInstance))
		{
			return IsValid(SaveGameInstance->CreateNewSaveGame());
		}
	}
	return false;
}

UMyGameSave* USaveSystemBlueprintFL::LoadOrCreateSaveGame(UObject* WorldContextObject, FString SlotName,
                                                          int32 UserIndex)
{
	if (!IsValid(WorldContextObject)) return nullptr;
	if (const UWorld* World = WorldContextObject->GetWorld(); IsValid(World))
	{
		if (USaveGameInstanceSubsystem* SaveGameInstance = World->GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>(); IsValid(SaveGameInstance))
		{
			SaveGameInstance->LoadOrCreateSaveGame(SlotName, UserIndex, false);
			return SaveGameInstance->CurrentSaveGame;
		}
	}
	
	return nullptr;
}

USaveGameInstanceSubsystem* USaveSystemBlueprintFL::GetSaveGameInstanceSubsystem(UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject)) return nullptr;
	if (const UWorld* World = WorldContextObject->GetWorld(); IsValid(World))
	{
		return World->GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>();
	}
	return nullptr;
}

UMyGameSave* USaveSystemBlueprintFL::GetCurrentSaveGame(UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject)) return nullptr;
	if (const UWorld* World = WorldContextObject->GetWorld(); IsValid(World))
	{
		if (const USaveGameInstanceSubsystem* SaveGameInstance = World->GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>();
			IsValid(SaveGameInstance))
		{
			return SaveGameInstance->CurrentSaveGame;
		}
	}
	return nullptr;
}

