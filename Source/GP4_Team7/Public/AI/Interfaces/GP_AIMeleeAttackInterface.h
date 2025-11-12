// Galla

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GP_AIMeleeAttackInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGP_AIMeleeAttackInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIMeleeAttackFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAISecondMeleeAttackFinishedSignature);

class GP4_TEAM7_API IGP_AIMeleeAttackInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MeleeAttack")
	bool CanPerformMeleeAttack() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MeleeAttack")
	void PerformMeleeAttack(AActor* InstigatorActor, AActor* Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MeleeAttack")
	void StopPerformMeleeAttack();

	virtual FOnAIMeleeAttackFinishedSignature& GetAIMeleeAttackFinishedDelegate() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MeleeAttack")
	void OnMeleeAttackFinished();

	// second melee attack
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MeleeAttack")
	void PerformSecondMeleeAttack(AActor* InstigatorActor, AActor* Target);

	virtual FOnAISecondMeleeAttackFinishedSignature& GetAISecondMeleeAttackFinishedDelegate() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MeleeAttack")
	void OnSecondMeleeAttackFinished();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MeleeAttack")
	void StopPerformSecondMeleeAttack();
};
