// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UStatAttributesComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UStatAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatAttributesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Stats")
	float CalculateDamage(UStatAttributesComponent* Other, float Damage) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(DisplayName="Gunfire Defense Percent"))
	float GunfireDefPct = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(DisplayName="Melee Defense Percent"))
	float MeleeDefPct = 10.0f;
};
