// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI/Interfaces/GP_AIMeleeAttackInterface.h"
#include "GP_AIMeleeAttackComponent.generated.h"


UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API UGP_AIMeleeAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UGP_AIMeleeAttackComponent();

protected:

	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeAttack")
	float Cooldown = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeAttack")
	float MinMeleeAttackDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeAttack")
	float MaxMeleeAttackDamage = 50.f;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool CanPerformMeleeAttack_Internal() const;

	UFUNCTION(BlueprintCallable, Category = "MeleeAttack")
	bool CanPerformMeleeAttack();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MeleeAttack")
	void PerformMeleeAttack(AActor* InstigatorActor, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "MeleeAttack")
	virtual void StopPerformMeleeAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MeleeAttack")
	void ApplyMeleeAttackDamage(AActor* Target, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	bool IsDamageDone() const { return bIsDamageDone; };

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	void SetIsDamageDone(bool IsDamageDone) { bIsDamageDone = IsDamageDone; };

	UPROPERTY(BlueprintAssignable, Category = "MeleeAttack")
	FOnAIMeleeAttackFinishedSignature OnMeleeAttackFinished;

	FOnAIMeleeAttackFinishedSignature& GetAIMeleeAttackFinishedDelegate();

protected:

	UFUNCTION()
	void OnStartGlobalNormalTime();

	UFUNCTION()
	void OnStopGlobalNormalTime(float Time);

	virtual void ResetCooldown();

	bool bIsDamageDone = false;
	bool bOnCooldownAfterMeleeAttack = false;
	bool bOnMeleeAttack = false;

	FTimerHandle CooldownTimerHandle;

	float ElapsedCooldownTime = 0.f;
	float TimeDilation = 1.0f;

};
