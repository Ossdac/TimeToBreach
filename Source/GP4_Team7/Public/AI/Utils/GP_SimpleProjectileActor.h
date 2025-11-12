// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectPool/PooledObject.h"
#include "GP_SimpleProjectileActor.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class GP4_TEAM7_API AGP_SimpleProjectileActor : public APooledObject
{
	GENERATED_BODY()
	
public:	

	AGP_SimpleProjectileActor();

	void SetShotDirection(const FVector& Direction);
	void SetStartPosition(const FVector& Position) { StartPosition = Position; }
	void SetActorRotator(const FRotator& Rotator) { ActorRotator = Rotator; }

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComponent;

	/*UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;*/

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float DamageRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float DamageAmount = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	bool DoFullDamage = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float LifeSeconds = 5.0f;

private:

	FVector ShotDirection;
	FVector StartPosition;
	FRotator ActorRotator;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;
	FVector GetProjectileMovementDirection();
};
