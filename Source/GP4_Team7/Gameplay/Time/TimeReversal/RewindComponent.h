// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ActorComponent.h"
#include "Containers/RingBuffer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameModes/GameplayGamemode.h"
#include "StructUtils/InstancedStruct.h"
#include "RewindComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRewindStarted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRewindCompleted);

DECLARE_DYNAMIC_DELEGATE_OneParam(FRewindFloatDelegate, float, value);


USTRUCT(BlueprintType)
struct FRewindEvent
{
	GENERATED_BODY()
	
	
	TFunction<void()> Function; 

	FRewindEvent() {}
	FRewindEvent(const TFunction<void()>& InMethod) : Function(InMethod) {}
};

USTRUCT(BlueprintType)
struct FRewindBPFloatEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FRewindFloatDelegate Delegate;

	float StoredValue = 0.f;

	FRewindBPFloatEvent() {}
	FRewindBPFloatEvent(const FRewindFloatDelegate& InDelegate, float InValue) 
		: Delegate(InDelegate), StoredValue(InValue) {}
};

template<typename T>
struct TRewindEventNode
{
	float Key;
	T RewindEvent;
	TRewindEventNode* Next = nullptr;
	TRewindEventNode* Prev = nullptr;
};

template<typename T>
class TRewindEventStructure
{
private:
	TRewindEventNode<T>* Head = nullptr;
	TRewindEventNode<T>* Tail = nullptr;

public:
	// Add a key-value pair (must be >= last key)
	void Add(float Key, const T& RewindEvent)
	{
		// Ensure ordering
		if (Head == nullptr)
		{
			Head = new TRewindEventNode<T>();
			Head->Key = Key;
			Head->RewindEvent = RewindEvent;
			Tail = Head;
			return;
		}
		if (Key < Tail->Key)
		{
			DeleteTail();
			Add(Key, RewindEvent);
			return;
		}
		TRewindEventNode<T>* NewNode = new TRewindEventNode<T>();
		NewNode->Key = Key;
		NewNode->RewindEvent = RewindEvent;
		NewNode->Prev = Tail;
		Tail->Next = NewNode;
		Tail = NewNode;
	}

	// Find the value at the largest key <= SearchKey
	// Returns nullptr if none exists
	const T* FindFloor(float SearchKey, float& UsedKey) const
	{
		TRewindEventNode<T>* Current = Tail;
		while (Current != nullptr)
		{
			if (Current->Key <= SearchKey)
			{
				UsedKey = Current->Key;
				return &Current->RewindEvent;
			}
			Current = Current->Prev;
		}
		return nullptr;
	}

	const T* FindFloor(float SearchKey) const
	{
		TRewindEventNode<T>* Current = Tail;
		while (Current != nullptr)
		{
			if (Current->Key <= SearchKey)
			{
				return &Current->RewindEvent;
			}
			Current = Current->Prev;
		}
		return nullptr;
	}

	T* FindFloorAndDeleteRoof(float SearchKey)
	{
		{
			TRewindEventNode<T>* Current = Tail;
			while (Current != nullptr)
			{
				if (Current->Key > SearchKey)
				{
					TRewindEventNode<T>* ToDelete = Current;
					Current = Current->Prev;
					if (Current)
					{
						Current->Next = nullptr;
					}
					else
					{
						Head = nullptr;
						Tail = nullptr;
					}
					Tail = Current;

					delete ToDelete;
				}
				else
				{
					return &Current->RewindEvent;
				}
			}
			return nullptr;
		}
	}

	TArray<T*> FindAllDownToAndDeleteAfter(float SearchKey)
	{
		TArray<T*> Events;
		while (Tail != nullptr)
		{
			if (Tail->Key >= SearchKey)
			{
				Events.Add(&Tail->RewindEvent);
				DeleteTail();
			}
			else
			{
				break;
			}
		}
		return Events;
	}

	TArray<T*> FindAllDownTo(float SearchKey)
	{
		TArray<T*> Events;
		TRewindEventNode<T>* Current = Tail;

		while (Current != nullptr && Current->Key >= SearchKey)
		{
			Events.Add(&Current->RewindEvent);
			Current = Current->Prev;
		}

		return Events;
	}

	void DeleteAllDownTo(float SearchKey)
	{
		while (Tail != nullptr && Tail->Key >= SearchKey)
		{
			DeleteTail();
		}
	}

	void DeleteAllUpTo(float SearchKey)
	{
		while (Head != nullptr && Head->Prev != nullptr && Head->Prev->Key <= SearchKey)
		{
			DeleteHead();
		}
	}
	
	void DeleteFloor(float Key)
	{
		while (Head != nullptr && Head->Key < Key)
		{
			DeleteHead();
		}
	}

	void Clear()
	{
		while (Head != nullptr)
		{
			DeleteHead();
		}
	}

	void DeleteHead()
	{
		if (Head)
		{
			TRewindEventNode<T>* ToDelete = Head;
			Head = Head->Next;
			if (Head)
			{
				Head->Prev = nullptr;
			}
			else
			{
				Tail = nullptr;
			}
			delete ToDelete;
		}
	}

	void DeleteTail()
	{
		if (Tail)
		{
			TRewindEventNode<T>* ToDelete = Tail;
			Tail = Tail->Prev;
			if (Tail)
			{
				Tail->Next = nullptr;
			}
			else
			{
				Head = nullptr;
			}
			delete ToDelete;
		}
	}
};

USTRUCT(BlueprintType)
struct FAbilityCooldownRecord
{
	GENERATED_BODY()
	
	UPROPERTY()
	TWeakObjectPtr<UGameplayAbility> Ability;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> Effect;
	
	UPROPERTY()
	float LastActivationTime  = 0.0f;
	
	UPROPERTY()
	float CooldownDuration = 0.0f;
};



USTRUCT()
struct FTransformAndVelocitySnapshot
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	float TimeStamp;
	UPROPERTY(Transient)
	float TimeSinceLastSnapshot;
	UPROPERTY(Transient)
	FTransform Transform;
	UPROPERTY(Transient)
	FVector Velocity;
	UPROPERTY(Transient)
	FVector AngularVelocity;
};

// Snapshot of movement state that drives animation (velocity, mode)
// Used to correctly rewind and resume character animation.
USTRUCT()
struct FAnimationSnapshot
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	float TimeStamp;
	UPROPERTY(Transient)
	float TimeSinceLastSnapshot;
	UPROPERTY(Transient)
	FVector MovementVelocity;
	TEnumAsByte<EMovementMode> MovementMode = MOVE_None;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_TEAM7_API URewindComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URewindComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "Rewind")
	float SnapshotFrequencySeconds = 1.0f / 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Rewind")
	bool bSnapshotAnimationVariables = false;

	UPROPERTY(EditDefaultsOnly, Category = "Rewind")
	bool bPauseAnimationDuringTimeScrubbing = false;

	UPROPERTY(EditDefaultsOnly, Category = "Rewind")
	bool bHasAbilitySystem = true;

	UPROPERTY(EditDefaultsOnly, Category = "Rewind")
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Rewind|GAS")
	TSubclassOf<UGameplayEffect> CooldownGameplayEffectTemplate;

	UPROPERTY(BlueprintAssignable, Category = "Rewind")
	FOnRewindStarted OnRewindStarted;

	UPROPERTY(BlueprintAssignable, Category = "Rewind")
	FOnRewindCompleted OnRewindCompleted;
	
	void RecordEvent(const TFunction<void()>& Function);

	UFUNCTION(BlueprintCallable)
	void RecordBPFloatEvent(const FRewindFloatDelegate& Delegate, float Value);

	// UFUNCTION(BlueprintCallable, Category="Rewind")
	// void UpdateAbilityCooldown(UGameplayAbility* Ability, float CooldownDuration);
	
	UFUNCTION(BlueprintCallable, Category = "Rewind")
	bool IsRewinding() const { return bIsRewinding; }

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	float GetRewindSpeed() const { return RewindSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void SetRewindSpeed(float Speed) { RewindSpeed = Speed; }

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	bool IsRewindingEnabled() const { return bIsRewindingEnabled; }

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void SetIsRewindingEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void SetIsRecording(bool bEnabled);

	// Find last value <= Time
	const FInstancedStruct* GetUninterpolatable(FName SourceId, float Time) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Rewind")
	bool bIsRewinding = false;

private:
	UPROPERTY(VisibleAnywhere, Category = "Rewind")
	bool bIsRewindingEnabled = true;

	TRingBuffer<FTransformAndVelocitySnapshot> TransformAndVelocityRecentHistory;

	TRingBuffer<FAnimationSnapshot> AnimationRecentHistory;


	TRewindEventStructure<FRewindEvent> EventTimeline;

	TRewindEventStructure<FRewindBPFloatEvent> BPEventTimeline;

	UPROPERTY(Transient)
	TArray<FAbilityCooldownRecord> AbilityCooldowns;

	// UPROPERTY(Transient)
	// TMap<FName, FUninterpolatableVariableTimeline> UninterpolatableTimelines;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	uint32 MaxSnapshots = 1;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	float TimeSinceSnapshotsChanged = 0.0f;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	float StopRewindAt = MAX_FLT;

	UPROPERTY (Transient, VisibleAnywhere, Category = "Rewind|Debug")
	float LocalTimeFromStart = 0.0f;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	float RewindSpeed = 1.0f;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	int32 LatestSnapshotIndex = -1;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	int32 FramesSinceLastSnapshot = 0;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	bool bTimedRewindActive = false;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	bool bIsRecording = true;

	bool bStopRewind = true;
	
	// Root primitive component on owner, if one exists
	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	UPrimitiveComponent* OwnerRootComponent;

	// Movement component on owner, if one exists
	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	UCharacterMovementComponent* OwnerMovementComponent;

	// Skeletal mesh component on owner, if one exists
	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	USkeletalMeshComponent* OwnerSkeletalMesh;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	UAnimInstance* OwnerAnimInstance;

	// Whether time manipulation paused physics
	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	bool bPausedPhysics = false;

	// Whether time manipulation paused animation
	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	bool bPausedAnimation = false;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	bool bAnimationsPausedAtStartOfTimeManipulation = false;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Rewind|Debug")
	AGameplayGamemode* GameMode;

	// Called when rewinding starts
	UFUNCTION()
	void OnGlobalRewindStarted();

	UFUNCTION()
	void OnGlobalRewindSeconds(float Seconds);

	UFUNCTION()
	void LocalRewind();

	UFUNCTION(BlueprintCallable)
	void LocalRewindSeconds(float Seconds);

	UFUNCTION(BlueprintCallable)
	void LocalRewindSecondsAtSpeed(float Seconds, float Speed);

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetPathRewindSeconds(float Seconds, FRotator& Rotation);

	UFUNCTION(BlueprintCallable)
	FVector GetEndPositionRewindSecond(float Seconds);
	
	// Called when rewinding completes
	UFUNCTION()
	void OnGlobalRewindCompleted();

	UFUNCTION()
	void OnLocalRewindCompleted();

	void InitializeRingBuffers(float MaxRewindSeconds);

	void RecordSnapshot(float DeltaTime);

	void EraseFutureSnapshots();

	void PlaySnapshots(float DeltaTime, bool bRewinding);

	bool TryStartRewinding();

	bool TryStopRewinding();

	void PausePhysics();

	void UnpausePhysics();

	void PauseAnimation();

	void UnpauseAnimation();

	// Helper function for PlaySnapshots/PauseTime that handle cases where there are insufficient snapshots to interpolate
	bool HandleInsufficientSnapshots();

	void InterpolateAndApplySnapshots();

	FTransformAndVelocitySnapshot BlendSnapshots(
		const FTransformAndVelocitySnapshot& A,
		const FTransformAndVelocitySnapshot& B,
		float Alpha, bool bForVisual);

	FAnimationSnapshot BlendSnapshots(
		const FAnimationSnapshot& A,
		const FAnimationSnapshot& B,
		float Alpha);

	// Applies the provided transform and velocity snapshot to the owner
	void ApplySnapshot(const FTransformAndVelocitySnapshot& Snapshot, bool bApplyPhysics);

	// Applies the provided movement velocity and movement mode snapshot to the owner
	void ApplySnapshot(const FAnimationSnapshot& Snapshot, bool bApplyTimeDilationToVelocity);

	void ApplyEventRecordings(float Time);
};
