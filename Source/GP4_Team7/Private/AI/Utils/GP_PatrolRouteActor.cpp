// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utils/GP_PatrolRouteActor.h"
#include "Components/SplineComponent.h"

DEFINE_LOG_CATEGORY_STATIC(GP_PatrolRouteLog, All, All);

AGP_PatrolRouteActor::AGP_PatrolRouteActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	SplineComponent->SetupAttachment(GetRootComponent());
}

void AGP_PatrolRouteActor::BeginPlay()
{
	Super::BeginPlay();

	check(SplineComponent);
}

void AGP_PatrolRouteActor::IncrementPatrolPath()
{
	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	if (NumPoints == 0)
	{
		UE_LOG(GP_PatrolRouteLog, Warning, TEXT("Spline has no points!"));
		return;
	}

	// TODO: add for random point 

	if (SplineComponent->IsClosedLoop())
	{
		PatrolIndex = (PatrolIndex + 1) % NumPoints;
	}
	else
	{
		PatrolIndex += CurrentDirection;
		if (PatrolIndex >= NumPoints - 1)
		{
			PatrolIndex = NumPoints - 1;
			CurrentDirection = PatrolDirection.Y;
		}
		else if(PatrolIndex <= 0)
		{
			PatrolIndex = 0;
			CurrentDirection = PatrolDirection.X;
		}
	}
}

FVector AGP_PatrolRouteActor::GetSplinePointAsWorldPosition() const
{
	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	if (NumPoints == 0 || PatrolIndex < 0 || PatrolIndex >= NumPoints)
	{
		UE_LOG(GP_PatrolRouteLog, Warning, TEXT("Invalid Patrol Index: %d"), PatrolIndex);
		return FVector::ZeroVector;
	}

	return SplineComponent->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}

FVector AGP_PatrolRouteActor::GetRandomSplinePointAsWorldPosition() const
{
	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	if (NumPoints == 0)
	{
		UE_LOG(GP_PatrolRouteLog, Warning, TEXT("Invalid Patrol Index: %d"), PatrolIndex);
		return FVector::ZeroVector;
	}

	int32 RandomIndex = FMath::RandRange(0, NumPoints - 1);

	if (NumPoints > 1 && RandomIndex == PatrolIndex)
	{
		RandomIndex = (RandomIndex + 1) % NumPoints;
	}

	return SplineComponent->GetLocationAtSplinePoint(RandomIndex, ESplineCoordinateSpace::World);
}