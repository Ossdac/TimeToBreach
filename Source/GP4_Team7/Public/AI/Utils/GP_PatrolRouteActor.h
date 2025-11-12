// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP_PatrolRouteActor.generated.h"

class USplineComponent;

UCLASS()
class GP4_TEAM7_API AGP_PatrolRouteActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AGP_PatrolRouteActor();

protected:

	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditDefaultsOnly, Category = "AI|Components")
	USplineComponent* SplineComponent;

	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	void IncrementPatrolPath();

	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	FVector GetSplinePointAsWorldPosition() const;

	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	FVector GetRandomSplinePointAsWorldPosition() const;

private:

	int32 PatrolIndex = 0;
	const FIntPoint PatrolDirection = FIntPoint(1, -1);
	int8 CurrentDirection = 1;

};
