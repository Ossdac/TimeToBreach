// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Animations/GP_StuggerPunchAnimNotifyState.h"
#include "Engine/DamageEvents.h"
#include "AI/GP_AICharacter.h"
#include "AI/GP_AIController.h"
#include "AI/Components/GP_HumanoidMeleeAttackComponent.h"

void UGP_StuggerPunchAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	PreviousHandLocation = MeshComp->GetSocketLocation(SocketName);
}

void UGP_StuggerPunchAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	AGP_AICharacter* AICharacter = Cast<AGP_AICharacter>(MeshComp->GetOwner());
	if (!AICharacter) return;

	UGP_HumanoidMeleeAttackComponent* AttackComponent = AICharacter->FindComponentByClass<UGP_HumanoidMeleeAttackComponent>();
	if (!AttackComponent || AttackComponent->IsDamageDone()) return;

	FVector CurrentLocation = MeshComp->GetSocketLocation(SocketName);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AICharacter);

	TArray<FHitResult> HitResults;
	bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(
		HitResults,
		PreviousHandLocation,
		CurrentLocation,
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(Radius),
		Params
	);

//#if WITH_EDITOR
//	DrawDebugSphere(MeshComp->GetWorld(), PreviousHandLocation, Radius, 12, FColor::Blue, false, 0.2f);
//	DrawDebugSphere(MeshComp->GetWorld(), CurrentLocation, Radius, 12, FColor::Red, false, 0.2f);
//	DrawDebugLine(MeshComp->GetWorld(), PreviousHandLocation, CurrentLocation, FColor::Green, false, 0.2f);
//#endif

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != AICharacter)
			{
				const auto AIController = Cast<AGP_AIController>(AICharacter->GetController());
				if (AIController && AIController->GetTeamAttitudeTowards(*HitActor) == ETeamAttitude::Hostile)
				{
					//UE_LOG(LogTemp, Display, TEXT("SweepHit: %s"), *HitActor->GetName());
					AttackComponent->ApplySecondMeleeAttackDamage(HitActor, AICharacter);
					AttackComponent->SetIsDamageDone(true);
					break;
				}
			}
		}
	}

	PreviousHandLocation = CurrentLocation;
}

void UGP_StuggerPunchAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	PreviousHandLocation = FVector::ZeroVector;
}
