// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/ObjectPool.h"


// Sets default values for this component's properties
UObjectPool::UObjectPool()
{
}

void UObjectPool::BeginPlay()
{
	Super::BeginPlay();

	if (PooledObjectClass != nullptr && GetWorld() != nullptr)
	{
		for (int i = 0; i < PoolSize; i++)
		{
			APooledObject* PoolableActor = GetWorld()->SpawnActor<APooledObject>(PooledObjectClass, InitialSpawnedObjectsLocation, FRotator().ZeroRotator);

			if (PoolableActor != nullptr)
			{
				PoolableActor->SetActive(false);
				PoolableActor->SetPoolIndex(i);
				PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &UObjectPool::OnPooledObjectDespawn);
				ObjectPool.Add(PoolableActor);
			}
		}
	}
}

APooledObject* UObjectPool::SpawnPooledObject(FVector InLocation, FRotator InRotation)
{
	for (APooledObject* PoolableActor : ObjectPool)
	{
		if (PoolableActor != nullptr && !PoolableActor->IsActive())
		{
			PoolableActor->TeleportTo(InLocation, InRotation);
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
			PoolableActor->SetActive(true);
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());
			
			return PoolableActor;
		}
	}

	if (SpawnedPoolIndexes.Num() > 0)
	{
		int PooledObjectIndex = SpawnedPoolIndexes[0];
		SpawnedPoolIndexes.Remove(PooledObjectIndex);
		APooledObject* PoolableActor = ObjectPool[PooledObjectIndex];

		if (PoolableActor != nullptr)
		{
			PoolableActor->SetActive(false);
			
			PoolableActor->TeleportTo(InLocation, InRotation);
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
			PoolableActor->SetActive(true);
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());

			return PoolableActor;
		}
	}
	
	return nullptr;
}

APooledObject* UObjectPool::SpawnPooledObjectWithDifferentLifeSpan(FVector InLocation, FRotator InRotation, float InLifeSpan)
{
	for (APooledObject* PoolableActor : ObjectPool)
	{
		if (PoolableActor != nullptr && !PoolableActor->IsActive())
		{
			PoolableActor->TeleportTo(InLocation, InRotation);
			PoolableActor->SetLifeSpan(InLifeSpan);
			PoolableActor->SetActive(true);
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());
			
			return PoolableActor;
		}
	}

	if (SpawnedPoolIndexes.Num() > 0)
	{
		int PooledObjectIndex = SpawnedPoolIndexes[0];
		SpawnedPoolIndexes.Remove(PooledObjectIndex);
		APooledObject* PoolableActor = ObjectPool[PooledObjectIndex];

		if (PoolableActor != nullptr)
		{
			PoolableActor->SetActive(false);
			
			PoolableActor->TeleportTo(InLocation, InRotation);
			PoolableActor->SetLifeSpan(InLifeSpan);
			PoolableActor->SetActive(true);
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());

			return PoolableActor;
		}
	}
	
	return nullptr;
}

void UObjectPool::OnPooledObjectDespawn(APooledObject* PoolActor)
{
	PoolActor->TeleportTo(InitialSpawnedObjectsLocation, FRotator(0, 0, 0));
	SpawnedPoolIndexes.Remove(PoolActor->GetPoolIndex());
}
