// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Components/GP_HumanoidMeleeAttackComponent.h"
#include "AI/Components/GP_AIMeleeAttackComponent.h"
#include "../../../GameModes/GameplayGamemode.h"

DEFINE_LOG_CATEGORY_STATIC(GP_HumanoidMeleeAttackComponentLog, All, All);

void UGP_HumanoidMeleeAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()) return;

	/*AGameplayGamemode* GameplayGameMode = Cast<AGameplayGamemode>(GetWorld()->GetAuthGameMode());
	if (!GameplayGameMode) return;

	GameplayGameMode->OnStartGlobalNormalTime.AddDynamic(this, &UGP_HumanoidMeleeAttackComponent::OnStartGlobalNormalTime);
	GameplayGameMode->OnStopGlobalNormalTime.AddDynamic(this, &UGP_HumanoidMeleeAttackComponent::OnStopGlobalNormalTime);*/
}

UGP_HumanoidMeleeAttackComponent::UGP_HumanoidMeleeAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGP_HumanoidMeleeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	if (bOnCooldownAfterSecondMeleeAttack)
	{
		if (ElapsedCooldownTime >= SecondMeleeCooldown)
		{
			ResetCooldown();
		}
		else
		{
			ElapsedCooldownTime += DeltaTime * TimeDilation;

			//UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("ElapsedCooldownTime = %f | SecondMeleeCooldown = %f | TimeDilation = %f"), ElapsedCooldownTime , SecondMeleeCooldown , TimeDilation);
		}
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGP_HumanoidMeleeAttackComponent::PerformMeleeAttack_Implementation(AActor* InstigatorActor, AActor* Target)
{
	if (!CanPerformMeleeAttack_Internal() || !Target) return;

	//UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("PerformHumanoidMeleeAttack"));

	bIsDamageDone = false;
	bOnMeleeAttack = true;
	//bOnCooldown = true;
	//GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UGP_HumanoidMeleeAttackComponent::ResetCooldown, Cooldown, false);
}

bool UGP_HumanoidMeleeAttackComponent::CanPerformMeleeAttack_Internal() const
{
	/*UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("bOnMeleeAttack = %s"), bOnMeleeAttack ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("bOnSecondMeleeAttack = %s"), bOnSecondMeleeAttack ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("bIsDamageDone = %s"), bIsDamageDone ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("bOnCooldownAfterMeleeAttack = %s"), bOnCooldownAfterMeleeAttack ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("bOnCooldownAfterSecondMeleeAttack = %s"), bOnCooldownAfterSecondMeleeAttack ? TEXT("TRUE") : TEXT("FALSE"));*/

	return !bOnSecondMeleeAttack && !bOnMeleeAttack && !bOnCooldownAfterMeleeAttack && !bOnCooldownAfterSecondMeleeAttack;
}

void UGP_HumanoidMeleeAttackComponent::PerformSecondMeleeAttack_Implementation(AActor* InstigatorActor, AActor* Target)
{
	if (!CanPerformMeleeAttack_Internal() || !Target) return;

	//UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("PerformSecondMeleeAttack"));

	bIsDamageDone = false;
	bOnSecondMeleeAttack = true;
	//bOnCooldownAfterSecondMeleeAttack = true;
	//GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UGP_HumanoidMeleeAttackComponent::ResetCooldown, SecondMeleeCooldown, false);
}

void UGP_HumanoidMeleeAttackComponent::ApplySecondMeleeAttackDamage_Implementation(AActor* Target, AActor* Instigator)
{
}

FOnAISecondMeleeAttackFinishedSignature& UGP_HumanoidMeleeAttackComponent::GetAISecondMeleeAttackFinishedDelegate()
{
	return OnSecondMeleeAttackFinished;
}

void UGP_HumanoidMeleeAttackComponent::StopPerformSecondMeleeAttack()
{
	//ResetCooldown();
	bOnCooldownAfterSecondMeleeAttack = true;
	bOnSecondMeleeAttack = false;
	OnSecondMeleeAttackFinished.Broadcast();
	//UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("StopPerformSecondMeleeAttack"));
}

void UGP_HumanoidMeleeAttackComponent::ResetCooldown()
{
	if (bOnCooldownAfterMeleeAttack)
	{
		bOnCooldownAfterMeleeAttack = false;
	}
	if (bOnCooldownAfterSecondMeleeAttack)
	{
		bOnCooldownAfterSecondMeleeAttack = false;
	}
	ElapsedCooldownTime = 0.f;
	bIsDamageDone = false;

	//GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);

	/*UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("bOnMeleeAttack = %s"), bOnMeleeAttack ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("bOnSecondMeleeAttack = %s"), bOnSecondMeleeAttack ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("bIsDamageDone = %s"), bIsDamageDone ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("bOnCooldownAfterMeleeAttack = %s"), bOnCooldownAfterMeleeAttack ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("bOnCooldownAfterSecondMeleeAttack = %s"), bOnCooldownAfterSecondMeleeAttack ? TEXT("TRUE") : TEXT("FALSE"));*/

	//UE_LOG(GP_HumanoidMeleeAttackComponentLog, Display, TEXT("ResetCooldown"));
}
