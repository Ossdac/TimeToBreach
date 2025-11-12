#pragma once
#include "InventoryItemData.h"
#include "ItemBox.generated.h"

USTRUCT(BlueprintType)
struct FItemBox
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int64 ItemId = -1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryItemData* ItemData = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemCount = 0;
};
