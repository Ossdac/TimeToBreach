// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Team7CharacterBase.h"
#include "Prototyping/SaveSystem/RegisterSaveComponent.h"
#include "Team7Character.generated.h"

UCLASS()
class GP4_TEAM7_API ATeam7Character : public ATeam7CharacterBase
{
	GENERATED_BODY()

public:
	ATeam7Character();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URegisterSaveComponent* RegisterSaveComponent;
private:
	void InitAbilityActorInfo();
	
};
