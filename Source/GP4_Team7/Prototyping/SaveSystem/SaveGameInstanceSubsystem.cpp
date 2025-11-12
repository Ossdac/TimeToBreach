// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameInstanceSubsystem.h"

#include "LogSaveCategory.h"
#include "MyGameSave.h"
#include "Savable.h"
#include "Kismet/GameplayStatics.h"


UMyGameSave* USaveGameInstanceSubsystem::CreateNewSaveGame()
{
	return CurrentSaveGame = Cast<UMyGameSave>(UGameplayStatics::CreateSaveGameObject(UMyGameSave::StaticClass()));
}

bool USaveGameInstanceSubsystem::SaveGame(const FString& SlotName, const int32 UserIndex)
{
	if (!CurrentSaveGame)
	{
		CreateNewSaveGame();
	}

	TArray<int> ShouldDeleteSavables;
	
	UE_LOG(LogSavable, Display, TEXT("Registered savables count: %d"), RegisteredSavables.Num());
	for (int i = 0; i < RegisteredSavables.Num(); ++i)
	{
		AActor* SavableActor = RegisteredSavables[i];
		if (IsValid(SavableActor) && SavableActor->GetClass()->ImplementsInterface(USavable::StaticClass()))
		{
			ISavable::Execute_SaveToSaveGame(SavableActor, CurrentSaveGame);
		}
		else
		{
			if (IsValid(SavableActor))
			{
				UE_LOG(LogSavable, Warning, TEXT("SavableActor %ls does not implement ISavable interface"), *SavableActor->GetName());
			}
			else
			{
				UE_LOG(LogSavable, Warning, TEXT("SavableActor is not valid and cannot be saved"));
				ShouldDeleteSavables.Add(i);
			}
		}
	}

	for (int i = ShouldDeleteSavables.Num() - 1; i >= 0; --i)
	{
		if (RegisteredSavables.IsValidIndex(ShouldDeleteSavables[i]))
		{
			RegisteredSavables.RemoveAt(ShouldDeleteSavables[i]);
		}
	}

	CurrentSaveGame->LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	
	FAsyncSaveGameToSlotDelegate Delegate;
	Delegate.BindLambda([](const FString& InSlotName, const int32 InUserIndex, bool bSuccess)
	{
		UE_LOG(LogSavable, Display, TEXT("Save to slot '%s' (UserIndex: %d) %s"),
			*InSlotName,
			InUserIndex,
			bSuccess ? TEXT("succeeded") : TEXT("failed"));
	});
	
	UGameplayStatics::AsyncSaveGameToSlot(CurrentSaveGame, SlotName, UserIndex, Delegate);
	return true;
}

bool USaveGameInstanceSubsystem::LoadSaveGame(const FString& SlotName, const int32 UserIndex, bool bSkipLevelLoad)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		if (USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex); IsValid(LoadedGame))
		{
			CurrentSaveGame = Cast<UMyGameSave>(LoadedGame);
			
			if (!IsValid(CurrentSaveGame)) return false;

			if (!bSkipLevelLoad && !CurrentSaveGame->LevelName.IsEmpty())
			{
				UGameplayStatics::OpenLevel(this, FName(*CurrentSaveGame->LevelName), true);
			}

			// UGameplayStatics::GetAllActorsWithInterface(this,
			// 	USavable::StaticClass(),
			// 	RegisteredSavables);
			//
			// for (AActor* SavableActor : RegisteredSavables)
			// {
			// 	if (IsValid(SavableActor) && SavableActor->GetClass()->ImplementsInterface(USavable::StaticClass()))
			// 	{
			// 		ISavable::Execute_LoadFromSaveGame(SavableActor, CurrentSaveGame);
			// 	}
			// }
			this->GameLoaded.Broadcast(SlotName, UserIndex, true);
			
			return true;
		}
	}

	return false;
}

UMyGameSave* USaveGameInstanceSubsystem::LoadOrCreateSaveGame(const FString& SlotName, const int32 UserIndex, bool bSkipLevelLoad)
{
	if (!LoadSaveGame(SlotName, UserIndex, bSkipLevelLoad))
	{
		return CreateNewSaveGame();
	}
	
	return CurrentSaveGame;
}

void USaveGameInstanceSubsystem::RegisterSavable(AActor* Savable)
{
	if (IsValid(Savable) && !RegisteredSavables.Contains(Savable))
	{
		UE_LOG(LogSavable, Display, TEXT("Registered savable"));
		RegisteredSavables.Add(Savable);
	}
}

void USaveGameInstanceSubsystem::UnregisterSavable(AActor* Savable)
{
	if (IsValid(Savable))
	{
		UE_LOG(LogSavable, Display, TEXT("Unregistered savable"));
		RegisteredSavables.Remove(Savable);
	}
}