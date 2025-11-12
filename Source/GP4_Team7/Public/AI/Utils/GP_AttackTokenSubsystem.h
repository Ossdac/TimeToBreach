// Galla

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GP_AttackTokenSubsystem.generated.h"

class AAIController;

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnTokenReservedSignature, AAIController*);

UCLASS()
class GP4_TEAM7_API UGP_AttackTokenSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(EditAnywhere, Category = "AttackToken")
	int32 MaxConcurrentAttackers = 2;

	bool RequestToken(AAIController* Requestor, int32 RequestedTokenAmount);

	void ReleaseToken(AAIController* Holder, int32 ReturnedTokenAmount);

	UFUNCTION(BlueprintCallable, Category = "AttackTokenSubsystem")
	bool CanRequestToken() const { return (CurrentTokenAmount > 0 /*&& !ActiveHolder.Get()*/); }

	//FOnTokenReservedSignature OnTokenReserved;

private:

	TArray<TWeakObjectPtr<AAIController>> ActiveHolders;

	int32 CurrentTokenAmount;

	void SetTokenAmount(int32 NewAmount);
};
