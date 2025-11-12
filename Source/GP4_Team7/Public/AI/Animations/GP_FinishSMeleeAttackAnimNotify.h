// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GP_FinishSMeleeAttackAnimNotify.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_FinishSMeleeAttackAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override
	{
		return TEXT("Finish Second Melee Attacking Notify");
	}
	
};
