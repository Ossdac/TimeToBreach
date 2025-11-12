// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemsDatabaseDataAsset.h"
#include "../LogInventoryCategory.h"

#if WITH_EDITOR
#include "UObject/SavePackage.h"
#endif

#if WITH_EDITOR
void UItemsDatabaseDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (!Items[i]) continue;
		Items[i]->UpdateItemID(i);
		UE_LOG(LogInventory, Display, TEXT("Updated ItemID for item %s to %lld"), *Items[i]->GetName(), Items[i]->ItemID);

		// Save the asset after updating the ItemID
		if (!Items[i]->MarkPackageDirty())
		{
			UE_LOG(LogInventory, Error, TEXT("Failed to mark package dirty for item %s"), *Items[i]->GetName());
		}
		if (UPackage* Package = Items[i]->GetPackage())
		{
			FSavePackageArgs SaveArgs;
			SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
			SaveArgs.SaveFlags = SAVE_NoError;
			
			const FString PackageName = Package->GetName();
			if (const FString PackageFilename = FPackageName::LongPackageNameToFilename(PackageName,
				FPackageName::GetAssetPackageExtension());
				!UPackage::SavePackage(Package, nullptr, *PackageFilename, SaveArgs))
			{
				UE_LOG(LogInventory, Error, TEXT("Failed to save package for item %s"), *Items[i]->GetName());
			}
		}
	}
}
#endif

UInventoryItemData* UItemsDatabaseDataAsset::GetItemByID(const int64 ID) const
{
	if (const uint8 Index = ID & 0xFF; Items.IsValidIndex(Index))
	{
		return Items[Index];
	}
	
	return nullptr;
}

UInventoryItemData* UItemsDatabaseDataAsset::GetItemByIndex(const uint8 Index) const
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index];
	}
	return nullptr;
}
