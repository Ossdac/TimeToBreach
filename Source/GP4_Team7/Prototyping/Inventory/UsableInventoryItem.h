#pragma once
#include "CoreMinimal.h"
#include "UsableInventoryItem.generated.h"

class UInventoryComponent;
class UInventoryItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemUsed,
	UInventoryItemData*, Item,
	UInventoryComponent*, InventoryComponent
	);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UUsableInventoryItem : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintAssignable, Category="Item")
	FOnItemUsed OnItemUsed;

	UFUNCTION(BlueprintCallable, Category="Item")
	void UseItem(UInventoryItemData* ItemData,
		UInventoryComponent* InventoryComponent) const;
};
