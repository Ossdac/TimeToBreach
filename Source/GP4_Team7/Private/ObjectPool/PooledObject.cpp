// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/PooledObject.h"


// Sets default values
APooledObject::APooledObject()
{
	LifeSpanTimer = 0;
	PrimaryActorTick.bCanEverTick = true;
}

void APooledObject::Deactivate()
{
	SetActive(false);
	OnPooledObjectDespawn.Broadcast(this);
}

void APooledObject::SetActive(bool bActive)
{
	bIsActive = bActive;
	LifeSpanTimer = 0;
	SetActorHiddenInGame(!bActive);
}

void APooledObject::SetLifeSpan(float InLifeSpan)
{
	LifeSpan = InLifeSpan;
}

void APooledObject::SetPoolIndex(int InPoolIndex)
{
	PoolIndex = InPoolIndex;
}

bool APooledObject::IsActive() const
{
	return bIsActive;
}

int APooledObject::GetPoolIndex() const
{
	return PoolIndex;
}

void APooledObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (LifeSpan == 0) return;
	LifeSpanTimer += DeltaSeconds;

	if (LifeSpanTimer > LifeSpan && bIsActive)
	{
		Deactivate();
	}
}

