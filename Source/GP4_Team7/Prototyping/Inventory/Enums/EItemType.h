#pragma once

#include "CoreMinimal.h"
#include "EItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	QuestItem UMETA(DisplayName = "Quest Item"),
	UsableItem UMETA(DisplayName = "Usable Item"),
};
