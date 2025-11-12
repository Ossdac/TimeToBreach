// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Animations/GP_FinishRangedAttackAnimNotify.h"
#include "AI/Interfaces/GP_RangedAttackInterface.h"
#include "AI/Components/GP_AIRangedAttackComponent.h"  

void UGP_FinishRangedAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

    if (Owner->Implements<UGP_RangedAttackInterface>())
    {
        if (UGP_AIRangedAttackComponent* RangedComp = Owner->FindComponentByClass<UGP_AIRangedAttackComponent>())
        {
            RangedComp->GetAIRangedAttackFinishedDelegate().Broadcast();
        }
        IGP_RangedAttackInterface::Execute_OnRangedAttackFinished(Owner);
        return;
    }
}