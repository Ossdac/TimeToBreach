// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/Time/TimeReversal/RewindComponent.h"
#include "GP_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTakenSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaggerSignature, float, StunDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, PreviousHealth, float, NextHealth);

UCLASS( BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API UGP_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UGP_HealthComponent();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewind Component")
	URewindComponent* RewindComponent = nullptr;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeathSignature OnDeath;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDamageTakenSignature OnDamageTaken;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStaggerSignature OnStagger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component", meta = (ClampMin = "0", ClampMax = "1000.0"))
	float MaxHealth = 100.f;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	float GetCurrentHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void RewindCurrentHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	float GetHealthPercent() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	bool IsHealthFull() const;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	bool TryToAddHealth(float HealthAmount);

private:

	UPROPERTY(VisibleAnywhere, Category = "Health Component")
	float Health = 0;
	
	bool bIsDead = false;

	
};
