// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemySaveStateSnapshotComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_TEAM7_API UEnemySaveStateSnapshotComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Transient)
	int64 EnemyID = -1;

public:
	// Sets default values for this component's properties
	UEnemySaveStateSnapshotComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void SaveDeathState();
};
