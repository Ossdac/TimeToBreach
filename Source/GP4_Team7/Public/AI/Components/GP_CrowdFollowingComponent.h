// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "GP_CrowdFollowingComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class GP4_TEAM7_API UGP_CrowdFollowingComponent : public UCrowdFollowingComponent
{
	GENERATED_BODY()

public:

	UGP_CrowdFollowingComponent(const FObjectInitializer& ObjectInitializer);

	virtual FVector GetCrowdAgentLocation() const override;
	virtual FVector GetCrowdAgentVelocity() const override;
	virtual void GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const override;
	virtual float GetCrowdAgentMaxSpeed() const override;

protected:
	
	virtual void FollowPathSegment(float DeltaTime) override;
	virtual void OnPathUpdated() override;

	
};
