// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "RewindableCharacter.h"
#include "GameFramework/Character.h"
#include "Prototyping/SaveSystem/Savable.h"
#include "Team7CharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class GP4_TEAM7_API ATeam7CharacterBase :
	public ARewindableCharacter,
	public IAbilitySystemInterface,
	public IGenericTeamAgentInterface,
	public ISavable
{
	GENERATED_BODY()

public:
	ATeam7CharacterBase();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	UFUNCTION(BlueprintCallable, Category = "Generic Team Id")
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override { TeamID = NewTeamID; };

	UFUNCTION(BlueprintCallable, Category = "Generic Team Id")
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, Category = "Generic Team Id")
	FGenericTeamId TeamID = 3;

private:
	
	
};
