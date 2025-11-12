// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Components/GP_AIMeleeAttackComponent.h"
#include "AI/Interfaces/GP_AIMeleeAttackInterface.h"
#include "GP_HumanoidMeleeAttackComponent.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_HumanoidMeleeAttackComponent : public UGP_AIMeleeAttackComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	UGP_HumanoidMeleeAttackComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PerformMeleeAttack_Implementation(AActor* InstigatorActor, AActor* Target) override;

	
	virtual bool CanPerformMeleeAttack_Internal() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondMeleeAttack")
	float MinSecondMeleeAttackDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondMeleeAttack")
	float MaxSecondMeleeAttackDamage = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondMeleeAttack")
	float SecondMeleeCooldown = 1.5f;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SecondMeleeAttack")
	void PerformSecondMeleeAttack(AActor* InstigatorActor, AActor* Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SecondMeleeAttack")
	void ApplySecondMeleeAttackDamage(AActor* Target, AActor* Instigator);

	UPROPERTY(BlueprintAssignable, Category = "SecondMeleeAttack")
	FOnAISecondMeleeAttackFinishedSignature OnSecondMeleeAttackFinished;

	FOnAISecondMeleeAttackFinishedSignature& GetAISecondMeleeAttackFinishedDelegate();

	UFUNCTION(BlueprintCallable, Category = "SecondMeleeAttack")
	virtual void StopPerformSecondMeleeAttack();

protected:

	bool bOnCooldownAfterSecondMeleeAttack = false;
	bool bOnSecondMeleeAttack = false;

	virtual void ResetCooldown() override;
};
