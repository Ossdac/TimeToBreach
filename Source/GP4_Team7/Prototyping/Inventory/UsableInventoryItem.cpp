#include "UsableInventoryItem.h"

void UUsableInventoryItem::UseItem(UInventoryItemData* ItemData,
		UInventoryComponent* InventoryComponent) const
{
	OnItemUsed.Broadcast(ItemData, InventoryComponent);
}
