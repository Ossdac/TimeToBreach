// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Components/GP_CrowdFollowingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"

UGP_CrowdFollowingComponent::UGP_CrowdFollowingComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/*SetCrowdSeparation(true);
	SetCrowdAnticipateTurns(true);
	SetCrowdSeparationWeight(50.f);
	SetCrowdAvoidanceRangeMultiplier(1.5f);
	SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);*/
}

FVector UGP_CrowdFollowingComponent::GetCrowdAgentLocation() const
{
	if (const AAIController* AIC = Cast<AAIController>(GetOwner()))
	{
		if (const ACharacter* C = Cast<ACharacter>(AIC->GetPawn()))
		{
			return C->GetActorLocation();
		}
	}

	return FVector::ZeroVector;
}

FVector UGP_CrowdFollowingComponent::GetCrowdAgentVelocity() const
{
	if (const AAIController* AIC = Cast<AAIController>(GetOwner()))
	{
		if (const ACharacter* C = Cast<ACharacter>(AIC->GetPawn()))
		{
			if (const UCharacterMovementComponent* M = C->GetCharacterMovement())
				return M->Velocity;
		}
	}
	return FVector::ZeroVector;
}

void UGP_CrowdFollowingComponent::GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const
{
	if (const AAIController* AIC = Cast<AAIController>(GetOwner()))
	{
		if (const ACharacter* C = Cast<ACharacter>(AIC->GetPawn()))
		{
			CylinderRadius = C->GetCapsuleComponent()->GetScaledCapsuleRadius();
			CylinderHalfHeight = C->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		}
	}
	else
	{
		Super::GetCrowdAgentCollisions(CylinderRadius, CylinderHalfHeight);
	}
}

float UGP_CrowdFollowingComponent::GetCrowdAgentMaxSpeed() const
{
	if (const AAIController* AIC = Cast<AAIController>(GetOwner()))
	{
		if (const ACharacter* C = Cast<ACharacter>(AIC->GetPawn()))
		{
			if (const UCharacterMovementComponent* M = C->GetCharacterMovement())
				return M->GetMaxSpeed();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("fuck"));
	return 300.f;
}

void UGP_CrowdFollowingComponent::FollowPathSegment(float DeltaTime)
{
	Super::FollowPathSegment(DeltaTime);
}

void UGP_CrowdFollowingComponent::OnPathUpdated()
{
	Super::OnPathUpdated();
}
