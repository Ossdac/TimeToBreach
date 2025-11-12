// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableInventoryItem.h"
#include "Engine/DataAsset.h"
#include "Enums/EItemType.h"
#include "InventoryItemData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GP4_TEAM7_API UInventoryItemData : public UDataAsset
{
	GENERATED_BODY()

	/// Returns a hash value generated from the item name.
	static uint32 GetHashByName(const FString& Name);

	
public:
	/// Generates a unique ItemID based on the item index in the database and its type.
	static int64 GetPackedItemId(uint8 Index, EItemType ItemType);

	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void Use(UObject* WorldContextObject);

	UPROPERTY(VisibleAnywhere, Category="Item", BlueprintReadOnly)
	int64 ItemID;

	UPROPERTY(EditAnywhere, Category="Item", BlueprintReadOnly)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, Category="Item", meta=(ClampMin = 1, ClampMax = 999), BlueprintReadOnly)
	int MaxStackSize;

	UPROPERTY(EditAnywhere, Category="Item", BlueprintReadOnly)
	FString ItemName;
	UPROPERTY(EditAnywhere, Category="Item", meta=(MultiLine=true), BlueprintReadOnly)
	FString ItemDescription;

	UPROPERTY(EditAnywhere, Category="Item", BlueprintReadOnly)
	TObjectPtr<UTexture> ItemIcon;

	void UpdateItemID(uint8 Index);

	UFUNCTION(BlueprintCallable, Category="Item")
	uint8 GetTypeIndex() const;
};
