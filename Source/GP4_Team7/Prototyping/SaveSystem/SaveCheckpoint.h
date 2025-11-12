// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
#include "SaveCheckpoint.generated.h"

UCLASS()
class GP4_TEAM7_API ASaveCheckpoint : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SaveCheckpoint", meta=(AllowPrivateAccess="true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, Transient)
	bool bCanActivateCheckpoint = false;

	UPROPERTY(VisibleAnywhere)
	float CooldownTimer = 3.0f;

	float CurrentCooldown = 0.0f;

public:
	// Sets default values for this actor's properties
	ASaveCheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Category="SaveCheckpoint")
	void ActivateCheckpoint(AActor* ActorA, AActor* ActorB);

	UFUNCTION(BlueprintCallable, Category="SaveCheckpoint")
	void DeactivateCheckpoint(AActor* ActorA, AActor* ActorB);
};
