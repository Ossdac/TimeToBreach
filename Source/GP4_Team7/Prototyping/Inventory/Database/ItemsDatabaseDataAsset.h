// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Prototyping/Inventory/InventoryItemData.h"
#include "ItemsDatabaseDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM7_API UItemsDatabaseDataAsset : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Items Database")
	TArray<TObjectPtr<UInventoryItemData>> Items;
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UInventoryItemData* GetItemByID(int64 ID) const;
	UInventoryItemData* GetItemByIndex(uint8 Index) const;

};
