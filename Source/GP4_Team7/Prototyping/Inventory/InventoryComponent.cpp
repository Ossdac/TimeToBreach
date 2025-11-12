#include "InventoryComponent.h"

#include "LogInventoryCategory.h"
#include "Prototyping/SaveSystem/MyGameSave.h"
#include "Prototyping/SaveSystem/SaveGameInstanceSubsystem.h"

class USaveGameInstance;

void UInventoryComponent::PrintInventory() const
{
	UE_LOG(LogInventory, Display, TEXT("Inventory contains %d items:"), CollectedItems.Num());
	for (const auto& Pair : CollectedItems)
	{
		if (const UInventoryItemData* Item = FindItemById(Pair.Key); IsValid(Item))
		{
			UE_LOG(LogInventory, Display, TEXT("- %s (ID: %lld) x%d"), *Item->ItemName, Pair.Key, Pair.Value);
		}
		else
		{
			UE_LOG(LogInventory, Warning, TEXT("- Unknown Item (ID: %lld) x%d"), Pair.Key, Pair.Value);
		}
	}
}

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddItem(UInventoryItemData* Item, const int Amount)
{
	if (IsValid(Item) && !IsFull())
	{
		if (CollectedItems.Contains(Item->ItemID))
		{
			CollectedItems[Item->ItemID] += Amount;
		}
		else
		{
			CollectedItems.Add(Item->ItemID, Amount);
		}

		CollectedItems[Item->ItemID] = FMath::Clamp(CollectedItems[Item->ItemID], 1, Item->MaxStackSize);
		SaveItemSnapshot(Item->ItemID, CollectedItems[Item->ItemID]);
		
		PrintInventory();
		OnItemAdded.Broadcast(Item, CollectedItems.Num(), CollectedItems[Item->ItemID]);
		OnItemCountChanged.Broadcast(CollectedItems.Num());
		return true;
	}
	
	return false;
}

bool UInventoryComponent::AddItemById(const int64 ItemId)
{
	if (IsValid(ItemsDatabase) && !IsFull())
	{
		if (UInventoryItemData* Item = ItemsDatabase->GetItemByID(ItemId); IsValid(Item))
		{
			CollectedItems.Add(Item->ItemID);
			OnItemAdded.Broadcast(Item, CollectedItems.Num(), CollectedItems[Item->ItemID]);
			return true;
		}
	}
	
	return false;
}

bool UInventoryComponent::AddItemByTypeIndex(uint8 TypeIndex, int Amount)
{
	if (UInventoryItemData* Item = FindItemByIndex(TypeIndex); IsValid(Item))
	{
		if (IsFull()) return false;
		return AddItem(Item, Amount);
	}
	
	return false;
}

bool UInventoryComponent::RemoveItem(UInventoryItemData* Item, const int Amount)
{
	if (IsValid(Item))
	{
		if (!HasItem(Item)) return false;
		
		CollectedItems[Item->ItemID] -= Amount;
		if (CollectedItems[Item->ItemID] <= 0)
		{
			CollectedItems.Remove(Item->ItemID);
		}
		
		OnItemRemoved.Broadcast(Item, CollectedItems.Num(),
			CollectedItems.Contains(Item->ItemID)
			? CollectedItems[Item->ItemID]
			: 0);
		OnItemCountChanged.Broadcast(CollectedItems.Num());
		return true;
	}
	return false;
}

bool UInventoryComponent::RemoveItemByTypeIndex(uint8 TypeIndex, int Amount)
{
	if (UInventoryItemData* Item = FindItemByIndex(TypeIndex); IsValid(Item))
	{
		if (IsFull()) return false;
		return RemoveItem(Item, Amount);
	}
	
	return false;
}

bool UInventoryComponent::RemoveItemById(int64 ItemId)
{
	if (IsValid(ItemsDatabase))
	{
		if (UInventoryItemData* Item = ItemsDatabase->GetItemByID(ItemId); IsValid(Item))
		{
			const int Status = CollectedItems.Remove(Item->ItemID);
			OnItemRemoved.Broadcast(Item, CollectedItems.Num(),
				CollectedItems.Contains(Item->ItemID)
				? CollectedItems[Item->ItemID]
				: 0);
			OnItemCountChanged.Broadcast(CollectedItems.Num());
			return Status > 0;
		}
	}
	return false;
}

bool UInventoryComponent::HasItem(UInventoryItemData* Item) const
{
	if (!IsValid(Item)) return false;
	
	return CollectedItems.Contains(Item->ItemID);
}

bool UInventoryComponent::HasItemByTypeIndex(const uint8 TypeIndex, const EItemType ItemType) const
{
	return HasItemById(UInventoryItemData::GetPackedItemId(TypeIndex, ItemType));
}

bool UInventoryComponent::HasItemById(const int64 ItemId) const
{
	return CollectedItems.Contains(ItemId);
}

UInventoryItemData* UInventoryComponent::FindItemByTypeIndex(const uint8 TypeIndex, const EItemType ItemType) const
{
	if (!IsValid(ItemsDatabase)) return nullptr;
	return ItemsDatabase->GetItemByID(UInventoryItemData::GetPackedItemId(TypeIndex, ItemType));
}

UInventoryItemData* UInventoryComponent::FindItemById(const int64 ItemID) const
{
	if (!IsValid(ItemsDatabase)) return nullptr;
	return ItemsDatabase->GetItemByID(ItemID);
}

UInventoryItemData* UInventoryComponent::FindItemByIndex(const uint8 Index) const
{
	if (!IsValid(ItemsDatabase)) return nullptr;
	return ItemsDatabase->GetItemByIndex(Index);
}

bool UInventoryComponent::IsFull() const
{
	return CollectedItems.Num() >= MaxItems;
}

int32 UInventoryComponent::GetItemCount() const
{
	return CollectedItems.Num();
}

int32 UInventoryComponent::GetItemCountByTypeIndex(const uint8 TypeIndex)
{
	if (const UInventoryItemData* Item = FindItemByIndex(TypeIndex); IsValid(Item))
	{
		if (CollectedItems.Contains(Item->ItemID))
		{
			return CollectedItems[Item->ItemID];
		}
	}
	
	return 0;
}

bool UInventoryComponent::UseItem(UInventoryItemData* Item)
{
	if (IsValid(Item) && HasItem(Item))
	{
		return UseItemById(Item->ItemID);
	}
	
	return false;
}

bool UInventoryComponent::UseItemByTypeIndex(const uint8 TypeIndex)
{
	if (UInventoryItemData* Item = FindItemByIndex(TypeIndex); IsValid(Item))
	{
		if (HasItem(Item))
		{
			return UseItemById(Item->ItemID);
		}
	}
	
	return false;
}

bool UInventoryComponent::UseItemById(const int64 ItemId)
{
	if (HasItemById(ItemId))
	{
		if (UInventoryItemData* ItemData = FindItemById(ItemId); IsValid(ItemData))
		{
			ItemData->Use(this);
			return true;
		}
	}
	
	return false;
}

void UInventoryComponent::ClearInventory()
{
	CollectedItems.Empty();
	OnItemCountChanged.Broadcast(CollectedItems.Num());
}

TMap<int64, int32> UInventoryComponent::GetItemsAsMap() const
{
	TMap<int64, int32> ItemsMap;
	for (const auto& Pair : CollectedItems)
	{
		ItemsMap.Add(Pair.Key, Pair.Value);
	}
	return ItemsMap;
}

void UInventoryComponent::LoadItemsFromMap(const TMap<int64, int32>& ItemsMap)
{
	CollectedItems.Empty();
	for (const auto& Pair : ItemsMap)
	{
		if (UInventoryItemData* Item = FindItemById(Pair.Key); IsValid(Item))
		{
			CollectedItems.Add(Pair.Key, Pair.Value);
			OnItemAdded.Broadcast(Item, CollectedItems.Num(), Pair.Value);
		}
	}
	PrintInventory();
	OnItemCountChanged.Broadcast(CollectedItems.Num());
}

TArray<FItemBox> UInventoryComponent::GetItemsAsContainer()
{
	TArray<FItemBox> Items;
	for (const auto& Pair : CollectedItems)
	{
		if (UInventoryItemData* ItemData = FindItemById(Pair.Key); IsValid(ItemData))
		{
			FItemBox Container;
			Container.ItemId = Pair.Key;
			Container.ItemData = ItemData;
			Container.ItemCount = Pair.Value;
			Items.Emplace(Container);
		}
	}
	return Items;
}

void UInventoryComponent::SaveItemSnapshot(const int64 ItemID, const int32 ItemCount) const
{
	if (const UWorld* World = GetWorld(); IsValid(World))
	{
		if (!World->IsGameWorld()) return;
		if (const USaveGameInstanceSubsystem* SaveGameInstance = World->GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>();
			SaveGameInstance && IsValid(SaveGameInstance))
		{
			if (UMyGameSave* CurrentSaveGame = SaveGameInstance->CurrentSaveGame; IsValid(CurrentSaveGame))
			{
				CurrentSaveGame->CollectItem(ItemID, ItemCount);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent: CurrentSaveGame is not valid"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent: SaveGameInstance is not valid"));
		}
	}
}

// void UInventoryComponent::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	if (const UWorld* World = GetWorld(); IsValid(World))
// 	{
// 		if (!World->IsGameWorld()) return;
// 		if (const USaveGameInstance* SaveGameInstance = Cast<USaveGameInstance>(World->GetGameInstance());
// 			SaveGameInstance && IsValid(SaveGameInstance))
// 		{
// 			if (const AActor* Owner = GetOwner(); IsValid(Owner))
// 			{
// 				if (Owner->GetClass()->ImplementsInterface(USavable::StaticClass()))
// 				{
// 					if (UMyGameSave* CurrentSaveGame = SaveGameInstance->CurrentSaveGame;
// 						IsValid(CurrentSaveGame))
// 					{
// 						if (CurrentSaveGame->CollectedItems.Num() > 0)
// 						{
// 							LoadItemsFromMap(CurrentSaveGame->CollectedItems);
// 						}
// 						else
// 						{
// 							UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent: No items in CurrentSaveGame"));
// 						}
// 					}
// 					else
// 					{
// 						UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent: CurrentSaveGame is not valid"));
// 					}
// 				}
// 				else
// 				{
// 					UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent: Owner does not implement Savable interface"));
// 				}
// 			}
// 			else
// 			{
// 				UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent: Owner is not valid"));
// 			}
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent: SaveGameInstance is not valid"));
// 		}
// 	}
// }
