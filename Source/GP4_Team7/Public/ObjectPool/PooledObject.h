// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledObjectDespawn, APooledObject*, PoolActor);

UCLASS()
class GP4_TEAM7_API APooledObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APooledObject();

	FOnPooledObjectDespawn OnPooledObjectDespawn;

	UFUNCTION(BlueprintCallable, Category = "Pooled Object")
	void Deactivate();

	void SetActive(bool bActive);
	void SetLifeSpan(float InLifeSpan);
	void SetPoolIndex(int InPoolIndex);

	bool IsActive() const;
	int GetPoolIndex() const;

protected:
	virtual void Tick(float DeltaSeconds) override;
	
	bool bIsActive;
	float LifeSpan;
	float LifeSpanTimer;
	int PoolIndex;

	
};
