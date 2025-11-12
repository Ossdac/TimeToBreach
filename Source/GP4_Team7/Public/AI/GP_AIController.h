// Galla

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GP_AIController.generated.h"

class UGP_AIPerceptionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSightSensedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAICharacterDeathSignature);

UENUM(BlueprintType)
enum EAIState : uint8
{
	Passive = 0,
	Attacking,
	Stunned,
	Investigating,
	Dead
};

UCLASS()
class GP4_TEAM7_API AGP_AIController : public AAIController
{
	GENERATED_BODY()

public: 

	AGP_AIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void SetAIState(EAIState State);

	UFUNCTION(BlueprintCallable)
	EAIState GetAIState() const;

	UFUNCTION(BlueprintCallable)
	void ResetStunnedState();

	UFUNCTION(BlueprintCallable)
	void ClearTarget();

	UFUNCTION(BlueprintCallable)
	void HandleSensedSight(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void HandleSensedSound(FVector TargetLocation, AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void HandleSensedDamage(FVector TargetLocation, AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void HandleSensedPrediction(FVector TargetLocation, AActor* TargetActor);

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable, Category = "Generic Team Id")
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	UFUNCTION(BlueprintCallable, Category = "Generic Team Id")
	virtual FGenericTeamId GetGenericTeamId() const override;

	int32 GetReservedAttackTokens() const { return ReservedAttackTokens; }

	void SetReservedAttackTokens(int32 TokenAmount);

	UPROPERTY(BlueprintAssignable)
	FOnSightSensedSignature OnSightSensed;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAICharacterDeathSignature OnAICharacterDeath;

	void HandleDeath();

	void SetStunnedRemainedTime(float RemainedTime);

protected:

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UGP_AIPerceptionComponent* AIPerceptionComponent;*/

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Blackboard")
	FName CurrentAIStateKeyName = "CurrentAIState";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Blackboard")
	FName TargetActorKeyName = "TargetActor";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Blackboard")
	FName TargetLocationKeyName = "TargetLocation";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Blackboard")
	FName StunnedRemainedTimeKeyName = "StunnedRemainedTime";

	UPROPERTY(EditDefaultsOnly, Category = "AI|Aim")
	FName AimSocketName = "head";

	virtual void OnPossess(APawn* InPawn) override;

	virtual FVector GetFocalPointOnActor(const AActor* Actor) const override;

private:

	int32 ReservedAttackTokens = 0;
};
