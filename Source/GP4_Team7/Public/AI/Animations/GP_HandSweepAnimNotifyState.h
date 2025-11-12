// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GP_HandSweepAnimNotifyState.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_HandSweepAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sweep")
	FName SocketName = "HandGrip_R";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sweep")
	float Radius = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sweep")
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Pawn;

private:

	FVector PreviousHandLocation;
	FVector CurrentHandLocation;
	
};
