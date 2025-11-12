// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayGamemode.generated.h"

class UObjectPool;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartGlobalNormalTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopGlobalNormalTime, float, TimeDilation);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGlobalRewindStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGlobalRewindSeconds, float, Seconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGlobalRewindCompleted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGlobalFastForwardStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGlobalFastForwardCompleted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGlobalTimeScrubStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGlobalTimeScrubCompleted);

// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGlobalTimelineVisualizationEnabled);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGlobalTimelineVisualizationDisabled);

UCLASS()
class GP4_TEAM7_API AGameplayGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameplayGamemode();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UObjectPool* ProjectilePool;

	// Starts rewinding all actors with rewind components
	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void StartGlobalRewind();

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void StartGlobalRewindAtSpeed(float Speed) { GlobalRewindSpeed = Speed; StartGlobalRewind(); }

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void RewindForSeconds(float Seconds);

	// Stops rewinding all actors with rewind components
	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void StopGlobalRewind();

	// Starts fast forwarding all actors with rewind components
	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void StartGlobalFastForward();

	// Stops fast forwarding all actors with rewind components
	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void StopGlobalFastForward();

	// Toggles time scrubbing on all actors with rewind components
	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void ToggleTimeScrub();

	// Event for when global rewinds start
	UPROPERTY(BlueprintAssignable, Category = "Rewind")
	FOnGlobalRewindStarted OnGlobalRewindStarted;

	UPROPERTY(BlueprintAssignable, Category = "Rewind")
	FOnGlobalRewindSeconds OnGlobalRewindSeconds;

	// Event for when global rewinds stop
	UPROPERTY(BlueprintAssignable, Category = "Rewind")
	FOnGlobalRewindCompleted OnGlobalRewindCompleted;

	// Event for when global fast forwards start
	UPROPERTY(BlueprintAssignable, Category = "Rewind")
	FOnGlobalFastForwardStarted OnGlobalFastForwardStarted;

	// Event for when global fast forwards stop
	UPROPERTY(BlueprintAssignable, Category = "Rewind")
	FOnGlobalFastForwardCompleted OnGlobalFastForwardCompleted;

	// Event for when global time scrubbing starts
	UPROPERTY(BlueprintAssignable, Category = "Rewind")
	FOnGlobalTimeScrubStarted OnGlobalTimeScrubStarted;

	// Event for when global time scrubbing stops
	UPROPERTY(BlueprintAssignable, Category = "Rewind")
	FOnGlobalTimeScrubCompleted OnGlobalTimeScrubCompleted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Global Time")
	FOnStartGlobalNormalTime OnStartGlobalNormalTime;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Global Time")
	FOnStopGlobalNormalTime OnStopGlobalNormalTime;

	// Desired length of longest rewind; used to compute the rewind buffer size
	UPROPERTY(EditDefaultsOnly, Category = "Rewind")
	float MaxRewindSeconds = 10.0f;

	// Returns whether the component is currently rewinding
	UFUNCTION(BlueprintCallable, Category = "Rewind")
	bool IsGlobalTimeScrubbing() const { return bIsGlobalTimeScrubbing; };

	// Returns whether rewinding is currently enabled
	UFUNCTION(BlueprintCallable, Category = "Rewind")
	bool IsGlobalRewinding() const { return bIsGlobalRewinding; };

	// Returns whether fast forwarding is currently enabled
	UFUNCTION(BlueprintCallable, Category = "Rewind")
	bool IsGlobalFastForwarding() const { return bIsGlobalFastForwarding; };

	// Returns the current global time dilation
	UFUNCTION(BlueprintCallable, Category = "Rewind")
	float GetGlobalRewindSpeed() const { return GlobalRewindSpeed; }

private:
	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind")
	bool bIsGlobalTimeScrubbing = false;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind")
	bool bIsGlobalRewinding = false;
	
	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind")
	bool bIsGlobalFastForwarding = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Rewind")
	float GlobalRewindSpeed = 1.0f;
};
