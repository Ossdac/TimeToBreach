#pragma once
#include "CoreMinimal.h"
#include "RoomDiscoveredSnapshot.generated.h"

USTRUCT(BlueprintType)
struct FRoomDiscoveredSnapshot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 RoomID = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool bPlayerIsInRoom = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool bIsRoomDiscovered = false;
};
