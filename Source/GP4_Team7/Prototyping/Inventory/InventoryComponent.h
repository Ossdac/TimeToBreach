#pragma once
#include "InventoryItemData.h"
#include "ItemBox.h"
#include "Database/ItemsDatabaseDataAsset.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemAdded,
	UInventoryItemData*, Item, int32, NewItemCount, int32, StackCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemRemoved,
	UInventoryItemData*, Item, int32, NewItemCount, int32, StackCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemCountChanged, int32, NewItemCount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category="Inventory", Transient)
	TMap<int64, int> CollectedItems;
	
	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ClampMin = 1, ClampMax = 100))
	int32 MaxItems = 20;

	void PrintInventory() const;
	
public:
	UInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemAdded OnItemAdded;
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemRemoved OnItemRemoved;
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemCountChanged OnItemCountChanged;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	UItemsDatabaseDataAsset* ItemsDatabase;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(UInventoryItemData* Item, int Amount = 1);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItemById(int64 ItemId);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItemByTypeIndex(uint8 TypeIndex, int Amount);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(UInventoryItemData* Item, int Amount);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItemByTypeIndex(uint8 TypeIndex, int Amount);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItemById(int64 ItemId);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool HasItem(UInventoryItemData* Item) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool HasItemByTypeIndex(uint8 TypeIndex, EItemType ItemType) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool HasItemById(int64 ItemId) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	UInventoryItemData* FindItemByTypeIndex(uint8 TypeIndex, EItemType ItemType) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	UInventoryItemData* FindItemById(int64 ItemID) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	UInventoryItemData* FindItemByIndex(uint8 Index) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool IsFull() const;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 GetItemCount() const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 GetItemCountByTypeIndex(uint8 TypeIndex);

	UFUNCTION(BlueprintCallable, Category="Item")
	bool UseItem(UInventoryItemData* Item);

	UFUNCTION(BlueprintCallable, Category="Item")
	bool UseItemByTypeIndex(const uint8 TypeIndex);

	UFUNCTION(BlueprintCallable, Category="Item")
	bool UseItemById(const int64 ItemId);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void ClearInventory();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	TMap<int64, int32> GetItemsAsMap() const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void LoadItemsFromMap(const TMap<int64, int32>& ItemsMap);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<FItemBox> GetItemsAsContainer();
	// virtual void BeginPlay() override;

	void SaveItemSnapshot(int64 ItemID, int32 ItemCount) const;
};
