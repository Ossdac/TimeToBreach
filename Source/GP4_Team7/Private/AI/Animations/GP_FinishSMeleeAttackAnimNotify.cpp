// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Animations/GP_FinishSMeleeAttackAnimNotify.h"
#include "AI/Components/GP_HumanoidMeleeAttackComponent.h"
#include "AI/Interfaces/GP_AIMeleeAttackInterface.h"

DEFINE_LOG_CATEGORY_STATIC(GP_FinishSMeleeAttackAnimNotify, All, All);

void UGP_FinishSMeleeAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    if (Owner->Implements<UGP_AIMeleeAttackInterface>())
    {
        if (UGP_HumanoidMeleeAttackComponent* MeleeComp = Owner->FindComponentByClass<UGP_HumanoidMeleeAttackComponent>())
        {
            //UE_LOG(GP_FinishSMeleeAttackAnimNotify, Display, TEXT("GetAISecondMeleeAttackFinishedDelegate().Broadcast()"));
            MeleeComp->GetAISecondMeleeAttackFinishedDelegate().Broadcast();
        }

        IGP_AIMeleeAttackInterface::Execute_OnSecondMeleeAttackFinished(Owner);
        return;
    }
}
