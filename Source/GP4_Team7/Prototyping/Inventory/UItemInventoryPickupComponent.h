#pragma once
#include "InventoryComponent.h"
#include "InventoryItemData.h"
#include "UItemInventoryPickupComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UItemInventoryPickupComponent : public UActorComponent
{
	GENERATED_BODY()
	
	
	UPROPERTY(VisibleAnywhere, Category="Item")
	UInventoryItemData* ItemData;
	
public:
	// Sets default values for this component's properties
	UItemInventoryPickupComponent();
	
	UFUNCTION(BlueprintCallable, Category="Item")
	void PickupItem(UInventoryComponent* InventoryComponent) const;
};
