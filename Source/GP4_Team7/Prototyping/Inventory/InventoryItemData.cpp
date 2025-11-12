// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemData.h"

uint32 UInventoryItemData::GetHashByName(const FString& Name)
{
	return FCrc::StrCrc32(*Name);
}

int64 UInventoryItemData::GetPackedItemId(const uint8 Index, EItemType ItemType)
{
	return Index | static_cast<int64>(ItemType) << 32;
}

void UInventoryItemData::UpdateItemID(const uint8 Index)
{
	ItemID = GetPackedItemId(Index, ItemType);
}

uint8 UInventoryItemData::GetTypeIndex() const
{
	return ItemID & 0xFF;
}
