// Galla

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GP_RangedAttackInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGP_RangedAttackInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIRangedAttackFinishedSignature);

class GP4_TEAM7_API IGP_RangedAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RangedAttack")
	bool CanPerformRangedAttack() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RangedAttack")
	void PerformRangedAttack(AActor* InstigatorActor, AActor* Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RangedAttack")
	void StopPerformRangedAttack();

	virtual FOnAIRangedAttackFinishedSignature& GetAIRangedAttackFinishedDelegate() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "RangedAttack")
	void OnRangedAttackFinished();
};
