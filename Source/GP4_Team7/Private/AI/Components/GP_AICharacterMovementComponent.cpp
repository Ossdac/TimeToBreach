// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Components/GP_AICharacterMovementComponent.h"

UGP_AICharacterMovementComponent::UGP_AICharacterMovementComponent()
{
	bUseControllerDesiredRotation = true;
	NavMovementProperties.bUseAccelerationForPaths = true;
	NavMovementProperties.bUseFixedBrakingDistanceForPaths = true;
}
