// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GP_HealthComponent.h"

#include "Character/RewindableCharacter.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(GP_HealthComponentLog, All, All);

UGP_HealthComponent::UGP_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	
}


void UGP_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	// check if owner is RewindableCharacter and get its RewindComponent
	if (const auto OwnerCharacter = Cast<ARewindableCharacter>(GetOwner()))
	{
		RewindComponent = OwnerCharacter->RewindComponent;
		if (!RewindComponent)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("UGP_HealthComponent::BeginPlay: Owner is RewindableCharacter but has no RewindComponent!"));
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("UGP_HealthComponent::BeginPlay: Owner is not a RewindableCharacter!"));
	}
	Health = MaxHealth;
	if (RewindComponent)
	{
		RewindComponent->RecordEvent([RewindCurrentHealth(Health)]{});
	}
	OnHealthChanged.Broadcast(Health, Health);
	// SetCurrentHealth(MaxHealth);
}

void UGP_HealthComponent::SetCurrentHealth(float CurrentHealth)
{
	const auto NextHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
	const auto PreviousHealth = Health;
	Health = NextHealth;

	OnHealthChanged.Broadcast(PreviousHealth, NextHealth);
	if (RewindComponent)
	{
		TWeakObjectPtr<UGP_HealthComponent> WeakThis(this);
		RewindComponent->RecordEvent([WeakThis, PreviousHealth]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->RewindCurrentHealth(PreviousHealth);
			}
		});
	}
	//UE_LOG(GP_HealthComponentLog, Display, TEXT("SetCurrentHealth = %f"), Health);
}

void UGP_HealthComponent::RewindCurrentHealth(float NewHealth)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Rewinding Health!"));
	const float NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const float PreviousHealth = Health;
	
	Health = NextHealth;
	OnHealthChanged.Broadcast(PreviousHealth, NextHealth);
	//UE_LOG(GP_HealthComponentLog, Display, TEXT("RewindCurrentHealth = %f"), Health);
}

bool UGP_HealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

bool UGP_HealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull() || HealthAmount <= 0) return false;
	SetCurrentHealth(Health + HealthAmount);
	return true;
}

