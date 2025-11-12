// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.h"
#include "ObjectPool.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_TEAM7_API UObjectPool : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UObjectPool();

	

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	APooledObject* SpawnPooledObject(FVector InLocation, FRotator InRotation);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	APooledObject* SpawnPooledObjectWithDifferentLifeSpan(FVector InLocation, FRotator InRotation, float InLifeSpan);

	UFUNCTION()
	void OnPooledObjectDespawn(APooledObject* PoolActor);

	

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	TSubclassOf<APooledObject> PooledObjectClass;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	int PoolSize = 20;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float PooledObjectLifeSpan = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	FVector InitialSpawnedObjectsLocation = FVector::ZeroVector;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<APooledObject*> ObjectPool;
	TArray<int> SpawnedPoolIndexes;
	
};
