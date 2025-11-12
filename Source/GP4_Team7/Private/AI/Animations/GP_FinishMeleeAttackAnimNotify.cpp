// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Animations/GP_FinishMeleeAttackAnimNotify.h"
#include "AI/Components/GP_AIMeleeAttackComponent.h"
#include "AI/Interfaces/GP_AIMeleeAttackInterface.h"

void UGP_FinishMeleeAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    if (Owner->Implements<UGP_AIMeleeAttackInterface>())
    {
        if (UGP_AIMeleeAttackComponent* MeleeComp = Owner->FindComponentByClass<UGP_AIMeleeAttackComponent>())
        {
            MeleeComp->GetAIMeleeAttackFinishedDelegate().Broadcast();
        }
        IGP_AIMeleeAttackInterface::Execute_OnMeleeAttackFinished(Owner);
        return;
    }
}
