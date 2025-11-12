#include "UItemInventoryPickupComponent.h"

UItemInventoryPickupComponent::UItemInventoryPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UItemInventoryPickupComponent::PickupItem(UInventoryComponent* InventoryComponent) const
{
	if (IsValid(InventoryComponent) && IsValid(ItemData))
	{
		if (!InventoryComponent->IsFull() && !InventoryComponent->HasItem(ItemData))
		{
			InventoryComponent->AddItem(ItemData);
		}
	}
}
