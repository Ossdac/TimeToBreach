// Fill out your copyright notice in the Description page of Project Settings.


#include "RewindComponent.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Trace/Detail/Transport.h"


// Sets default values for this component's properties
URewindComponent::URewindComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Tick after movement is completed
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
}

void URewindComponent::BeginPlay()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(URewindComponent::BeginPlay);

	Super::BeginPlay();

	GameMode = Cast<AGameplayGamemode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		// Disable ticking if we can't find the rewind game mode; no hope of rewinding
		SetComponentTickEnabled(false);
		return;
	}
	// TODO: Grab future VisualizationComponent here

	// Grab owner's root component to manipulate physics during rewind
	OwnerRootComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (bSnapshotAnimationVariables)
	{
		OwnerMovementComponent = Character
			                         ? Cast<UCharacterMovementComponent>(Character->GetMovementComponent())
			                         : nullptr;

		OwnerAnimInstance = Character && Character->GetMesh()
			                    ? Character->GetMesh()->GetAnimInstance()
			                    : nullptr;
	}

	if (bPauseAnimationDuringTimeScrubbing) { OwnerSkeletalMesh = Character ? Character->GetMesh() : nullptr; }

	GameMode->OnGlobalRewindStarted.AddUniqueDynamic(this, &URewindComponent::OnGlobalRewindStarted);
	GameMode->OnGlobalRewindSeconds.AddUniqueDynamic(this, &URewindComponent::OnGlobalRewindSeconds);
	GameMode->OnGlobalRewindCompleted.AddUniqueDynamic(this, &URewindComponent::OnGlobalRewindCompleted);

	InitializeRingBuffers(GameMode->MaxRewindSeconds);
	EventTimeline = TRewindEventStructure<FRewindEvent>();
	BPEventTimeline = TRewindEventStructure<FRewindBPFloatEvent>();
}

void URewindComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(URewindComponent::TickComponent);

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if (bIsRecording) { LocalTimeFromStart += DeltaTime; }

	if (bIsRewinding) { PlaySnapshots(DeltaTime, true /*bRewinding*/); }
	else if (bIsRecording){ RecordSnapshot(DeltaTime); }

	if (bTimedRewindActive && bIsRewinding)
	{
		if (LocalTimeFromStart <= StopRewindAt || LocalTimeFromStart <= 0.0f || bStopRewind)
		{
			bTimedRewindActive = false;
			bStopRewind = true;
			OnLocalRewindCompleted();
		}
	}
}

void URewindComponent::RecordEvent(const TFunction<void()>& Function)
{
	if (bIsRewinding) { return; }
	EventTimeline.Add(LocalTimeFromStart, FRewindEvent(Function));
}

void URewindComponent::RecordBPFloatEvent(const FRewindFloatDelegate& Delegate, float Value)
{
	if (bIsRewinding) { return; }
	BPEventTimeline.Add(LocalTimeFromStart, FRewindBPFloatEvent(Delegate, Value));
}



void URewindComponent::SetIsRewindingEnabled(bool bEnabled)
{
	bIsRewindingEnabled = bEnabled;
	if (!bIsRewindingEnabled)
	{
		if (bIsRewinding) { OnGlobalRewindCompleted(); }
	}
	else
	{
		if (ensure(GameMode))
		{
			// Start rewinding if a global rewind is in progress
			if (!bIsRewinding && GameMode->IsGlobalRewinding()) { OnGlobalRewindStarted(); }
		}
	}
}

void URewindComponent::SetIsRecording(bool bEnabled)
{
	if (bIsRewinding) { return; }
	bIsRecording = bEnabled;
}


//TOD0: Make Global and Local version different, separate timelines and make global override local
void URewindComponent::OnGlobalRewindStarted()
{
	RewindSpeed = GameMode ? GameMode->GetGlobalRewindSpeed() : 1.0f;
	// Attempt to start rewinding; reset TimeSinceSnapshostsChanged if not time scrubbing
	if (TryStartRewinding())
	{
		// Notify event subscribers that rewind (and possibly time manipulation) started
		OnRewindStarted.Broadcast();
	}
}

void URewindComponent::OnGlobalRewindSeconds(float Seconds)
{
	LocalRewindSecondsAtSpeed(Seconds, GameMode->GetGlobalRewindSpeed());
}

void URewindComponent::LocalRewind()
{
	// Attempt to start rewinding; reset TimeSinceSnapshostsChanged if not time scrubbing
	if (TryStartRewinding())
	{
		// Notify event subscribers that rewind (and possibly time manipulation) started
		OnRewindStarted.Broadcast();
	}
}

void URewindComponent::LocalRewindSeconds(float Seconds)
{
	LocalRewindSecondsAtSpeed(Seconds, 1.0f);
}

void URewindComponent::LocalRewindSecondsAtSpeed(float Seconds, float Speed)
{
	if (TransformAndVelocityRecentHistory.IsEmpty()){return;}
	bTimedRewindActive = true;
	bStopRewind = false;
	RewindSpeed = Speed;
	StopRewindAt = LocalTimeFromStart - Seconds >= TransformAndVelocityRecentHistory.First().TimeStamp ?
		LocalTimeFromStart - Seconds : TransformAndVelocityRecentHistory.First().TimeStamp;
	LocalRewind();
}

TArray<FVector> URewindComponent::GetPathRewindSeconds(float Seconds, FRotator& Rotation)
{
	TArray<FVector> Path;
	if (TransformAndVelocityRecentHistory.Num() == 0) { return Path; }
	int CurrentIndex = LatestSnapshotIndex;
	while (CurrentIndex >= 0)
	{
		Seconds -= TransformAndVelocityRecentHistory[CurrentIndex].TimeSinceLastSnapshot;
		if (Seconds < 0.0f)
		{
			Seconds += TransformAndVelocityRecentHistory[CurrentIndex].TimeSinceLastSnapshot;
			break;
		}
		Path.Add(TransformAndVelocityRecentHistory[CurrentIndex].Transform.GetLocation());
		--CurrentIndex;
	}
	if (CurrentIndex <= 0 || Seconds <= 0)
	{
		Rotation = TransformAndVelocityRecentHistory[0].Transform.GetRotation().Rotator();
		return Path;
	}

	FTransformAndVelocitySnapshot EndSnapshot = BlendSnapshots(TransformAndVelocityRecentHistory[CurrentIndex],
															   TransformAndVelocityRecentHistory[CurrentIndex - 1],
															   Seconds / TransformAndVelocityRecentHistory[CurrentIndex
																   - 1].TimeSinceLastSnapshot, true);
	Rotation = EndSnapshot.Transform.GetRotation().Rotator();
	Path.Add(EndSnapshot.Transform.GetLocation());
	return Path;
}

FVector URewindComponent::GetEndPositionRewindSecond(float Seconds)
{
	if (TransformAndVelocityRecentHistory.Num() == 0) { return FVector(); }
	int CurrentIndex = LatestSnapshotIndex;
	while (CurrentIndex >= 0)
	{
		Seconds -= TransformAndVelocityRecentHistory[CurrentIndex].TimeSinceLastSnapshot;
		if (Seconds < 0.0f)
		{
			Seconds += TransformAndVelocityRecentHistory[CurrentIndex].TimeSinceLastSnapshot;
			break;
		}
		--CurrentIndex;
	}
	if (CurrentIndex <= 0 || Seconds <= 0) { return TransformAndVelocityRecentHistory[0].Transform.GetLocation(); }

	FTransformAndVelocitySnapshot EndSnapshot = BlendSnapshots(TransformAndVelocityRecentHistory[CurrentIndex],
	                                                           TransformAndVelocityRecentHistory[CurrentIndex - 1],
	                                                           Seconds / TransformAndVelocityRecentHistory[CurrentIndex
		                                                           - 1].TimeSinceLastSnapshot, false);
	return EndSnapshot.Transform.GetLocation();
}


//TOD0: Make Global and Local version different, separate timelines and make global override local
void URewindComponent::OnGlobalRewindCompleted()
{
	// Attempt to stop rewinding
	if (TryStopRewinding())
	{
		// Notify event subscribers that rewind (and possibly time manipulation) completed
		OnRewindCompleted.Broadcast();
	}
}

void URewindComponent::OnLocalRewindCompleted()
{
	// Attempt to stop rewinding
	if (TryStopRewinding())
	{
		// Notify event subscribers that rewind (and possibly time manipulation) completed
		OnRewindCompleted.Broadcast();
	}
	
}


void URewindComponent::InitializeRingBuffers(float MaxRewindSeconds)
{
	// Figure out how many snapshots we need to store
	MaxSnapshots = FMath::CeilToInt32(MaxRewindSeconds / SnapshotFrequencySeconds);

	// Make sure we're not accidentally allocating an obscene amount of memory
	constexpr uint32 OneMB = 1024 * 1024;
	constexpr uint32 ThreeMB = 3 * OneMB;
	if (!bSnapshotAnimationVariables)
	{
		uint32 SnapshotBytes = sizeof(FTransformAndVelocitySnapshot);
		uint32 TotalSnapshotBytes = MaxSnapshots * SnapshotBytes;
		ensureMsgf(
			TotalSnapshotBytes < OneMB,
			TEXT("Actor %s has rewind component that requested %d bytes of snapshots. Check snapshot frequency!"),
			*GetOwner()->GetName(),
			TotalSnapshotBytes);

		MaxSnapshots = FMath::Min(MaxSnapshots, static_cast<uint32>(OneMB / SnapshotBytes));
	}
	else
	{
		uint32 SnapshotBytes = sizeof(FTransformAndVelocitySnapshot) + sizeof(FAnimationSnapshot);
		uint32 TotalSnapshotBytes = MaxSnapshots * SnapshotBytes;
		ensureMsgf(
			TotalSnapshotBytes < ThreeMB,
			TEXT("Actor %s has rewind component that requested %d bytes of snapshots. Check snapshot frequency!"),
			*GetOwner()->GetName(),
			TotalSnapshotBytes);

		MaxSnapshots = FMath::Min(MaxSnapshots, static_cast<uint32>(ThreeMB / SnapshotBytes));
	}

	// Initialize buffer
	TransformAndVelocityRecentHistory.Reserve(MaxSnapshots);

	// Grab owner's root component to manipulate physics during rewind
	if (bSnapshotAnimationVariables && OwnerMovementComponent)
	{
		// Initialize buffer for movement component snapshots
		AnimationRecentHistory.Reserve(MaxSnapshots);
	}
}

void URewindComponent::RecordSnapshot(float DeltaTime)
{
	//TODO: Remove if array outside of max time, move to extended
	TRACE_CPUPROFILER_EVENT_SCOPE(URewindComponent::RecordSnapshot);

	FTransform Transform = GetOwner()->GetActorTransform();
	if (!TransformAndVelocityRecentHistory.IsEmpty())
	{
		if (Transform.Equals(TransformAndVelocityRecentHistory.Last().Transform)) {return;}
	}
	TimeSinceSnapshotsChanged += DeltaTime;

	LocalTimeFromStart += DeltaTime;

	// Early out if it's not yet time to record a snapshot
	if (TimeSinceSnapshotsChanged < SnapshotFrequencySeconds && TransformAndVelocityRecentHistory.Num() != 0)
	{
		return;
	}

	//TODO: Move old snapshots to extended history if outside of max rewind time
	// If the buffer is full, drop the oldest snapshot
	if (TransformAndVelocityRecentHistory.Num() == MaxSnapshots) { TransformAndVelocityRecentHistory.PopFront(); }

	
	FVector LinearVelocity = OwnerRootComponent ? OwnerRootComponent->GetPhysicsLinearVelocity() : FVector::Zero();
	FVector AngularVelocityInRadians = OwnerRootComponent
		                                   ? OwnerRootComponent->GetPhysicsAngularVelocityInRadians()
		                                   : FVector::Zero();
	LatestSnapshotIndex =
		TransformAndVelocityRecentHistory.Emplace(LocalTimeFromStart, TimeSinceSnapshotsChanged, Transform,
		                                          LinearVelocity, AngularVelocityInRadians);

	if (bSnapshotAnimationVariables && OwnerMovementComponent)
	{
		//TODO: Move old snapshots to extended history if outside of max rewind time
		// If the buffer is full, drop the oldest snapshot
		if (AnimationRecentHistory.Num() == MaxSnapshots) { AnimationRecentHistory.PopFront(); }

		// Record the movement velocity and movement mode
		FVector MovementVelocity = OwnerMovementComponent->Velocity;
		TEnumAsByte<EMovementMode> MovementMode = OwnerMovementComponent->MovementMode;
		int32 LatestMovementSnapshotIndex =
			AnimationRecentHistory.Emplace(LocalTimeFromStart, TimeSinceSnapshotsChanged, MovementVelocity,
			                               MovementMode);
		check(LatestSnapshotIndex == LatestMovementSnapshotIndex);
	}

	TimeSinceSnapshotsChanged = 0.0f;
}

void URewindComponent::EraseFutureSnapshots()
{
	// Pop snapshots until the latest snapshot is the last one in the buffer
	while (LatestSnapshotIndex < TransformAndVelocityRecentHistory.Num() - 1)
	{
		TransformAndVelocityRecentHistory.Pop();
	}

	if (bSnapshotAnimationVariables)
	{
		// Pop snapshots until the latest snapshot is the last one in the buffer
		while (LatestSnapshotIndex < AnimationRecentHistory.Num() - 1)
		{
			AnimationRecentHistory.Pop();
		}
	}
	EventTimeline.DeleteAllUpTo(TransformAndVelocityRecentHistory.Last().TimeStamp);
}

void URewindComponent::PlaySnapshots(float DeltaTime, bool bRewinding)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(URewindComponent::PlaySnapshots);

	UnpauseAnimation();

	if (HandleInsufficientSnapshots()) {return;}

	// Apply time dilation to delta time
	DeltaTime *= RewindSpeed;
	TimeSinceSnapshotsChanged += DeltaTime;
	// Latest snapshot index

	bool bReachedEndOfTrack = false;
	float LatestSnapshotTime = TransformAndVelocityRecentHistory[LatestSnapshotIndex].TimeSinceLastSnapshot;
	
	
	// Drop any snapshots that are too old to be relevant		
	while (LatestSnapshotIndex > 0 && TimeSinceSnapshotsChanged > LatestSnapshotTime)
	{
		TimeSinceSnapshotsChanged -= LatestSnapshotTime;
		LatestSnapshotTime = TransformAndVelocityRecentHistory[LatestSnapshotIndex].TimeSinceLastSnapshot;
		--LatestSnapshotIndex;
	}

	// If we don't have any snapshots in the future, we can't interpolate, so just snap to the latest snapshot
	if (LatestSnapshotIndex == TransformAndVelocityRecentHistory.Num() - 1)
	{
		if (TransformAndVelocityRecentHistory[0].TimeStamp < StopRewindAt)
		{
			// bStopRewind = true;;
			return;
		}
		ApplySnapshot(TransformAndVelocityRecentHistory[LatestSnapshotIndex], false /*bApplyPhysics*/);
		if (bSnapshotAnimationVariables)
		{
			ApplySnapshot(AnimationRecentHistory[LatestSnapshotIndex], true /*bApplyTimeDilationToVelocity*/);
		}
		ApplyEventRecordings(LocalTimeFromStart);
		return;
	}

	bReachedEndOfTrack = LatestSnapshotIndex == 0;


	// If we've reached the end of our track, clamp the interpolation and repause animation
	if (bReachedEndOfTrack)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Reached end of rewind track"));
		TimeSinceSnapshotsChanged = FMath::Min(TimeSinceSnapshotsChanged, LatestSnapshotTime);
		if (bAnimationsPausedAtStartOfTimeManipulation) { PauseAnimation(); }
		bStopRewind = true;
	}

	InterpolateAndApplySnapshots();
	ApplyEventRecordings(LocalTimeFromStart);
}


bool URewindComponent::TryStartRewinding()
{
	if (!bIsRewindingEnabled || bIsRewinding) { return false; }

	// Turn on requested time manipulation (i.e. bIsRewinding, bIsFastForwarding, bIsTimeScrubbing)
	bIsRewinding = true;
	bIsRecording = false;

	// Caller may want to maintain current interpolation (ex. during time scrubbing)
	TimeSinceSnapshotsChanged = 0.0f;

	// Physics simulation disabled during rewinding
	PausePhysics();

	// Check whether animations were paused when we started this time manipulation operation
	bAnimationsPausedAtStartOfTimeManipulation = bPausedAnimation;

	return true;
}

bool URewindComponent::TryStopRewinding()
{
	if (!bIsRewinding) { return false; }

	bIsRewinding = false;
	bIsRecording = true;

	EraseFutureSnapshots();

	return true;
}

void URewindComponent::PausePhysics()
{
	// Pause physics if it's simulating
	if (OwnerRootComponent && OwnerRootComponent->BodyInstance.bSimulatePhysics)
	{
		bPausedPhysics = true;
		OwnerRootComponent->SetSimulatePhysics(false);
	}
}

void URewindComponent::UnpausePhysics()
{
	{
		// Restart physics if simulation was paused
		if (!bPausedPhysics) { return; }

		check(OwnerRootComponent);
		bPausedPhysics = false;
		OwnerRootComponent->SetSimulatePhysics(true);
		OwnerRootComponent->RecreatePhysicsState();
	}
}

void URewindComponent::PauseAnimation()
{
	if (!bPauseAnimationDuringTimeScrubbing) { return; }

	check(OwnerSkeletalMesh);
	bPausedAnimation = true;
	OwnerSkeletalMesh->bPauseAnims = true;
}

void URewindComponent::UnpauseAnimation()
{
	if (!bPausedAnimation) { return; }

	check(OwnerSkeletalMesh);
	bPausedAnimation = false;
	OwnerSkeletalMesh->bPauseAnims = false;
}

bool URewindComponent::HandleInsufficientSnapshots()
{
	// Nothing to do if no snapshots are available
	check(!bSnapshotAnimationVariables || TransformAndVelocityRecentHistory.Num() == AnimationRecentHistory.Num());
	if (LatestSnapshotIndex < 0 || TransformAndVelocityRecentHistory.Num() == 0) { return true; }

	// If only one snapshot is available, snap to it
	if (TransformAndVelocityRecentHistory.Num() == 1)
	{
		if (TransformAndVelocityRecentHistory[0].TimeStamp < StopRewindAt)
		{
			bStopRewind = true;
			return true;
		}
		
		LocalTimeFromStart = TransformAndVelocityRecentHistory[0].TimeStamp;
		ApplySnapshot(TransformAndVelocityRecentHistory[0], false /*bApplyPhysics*/);
		if (bSnapshotAnimationVariables)
		{
			ApplySnapshot(AnimationRecentHistory[0], true /*bApplyTimeDilationToVelocity*/);
		}
		ApplyEventRecordings(LocalTimeFromStart);
		return true;
	}

	// Sanity check invariant
	check(LatestSnapshotIndex >= 0 && LatestSnapshotIndex < TransformAndVelocityRecentHistory.Num());
	return false;
}

void URewindComponent::InterpolateAndApplySnapshots()
{
	// Interpolate between the two relevant snapshots
	constexpr int MinSnapshotsForInterpolation = 2;
	check(TransformAndVelocityRecentHistory.Num() >= MinSnapshotsForInterpolation);
	check(LatestSnapshotIndex < TransformAndVelocityRecentHistory.Num() - 1 || LatestSnapshotIndex > 0);
	int PreviousIndex = LatestSnapshotIndex + 1;

	// Blend and apply transform and velocity snapshots (scoped to avoid variable shadowing)
	{
		const FTransformAndVelocitySnapshot& PreviousSnapshot = TransformAndVelocityRecentHistory[PreviousIndex];
		const FTransformAndVelocitySnapshot& NextSnapshot = TransformAndVelocityRecentHistory[LatestSnapshotIndex];
		ApplySnapshot(
			BlendSnapshots(PreviousSnapshot, NextSnapshot,
			               TimeSinceSnapshotsChanged / NextSnapshot.TimeSinceLastSnapshot, false),
			false /*bApplyPhysics*/);
	}

	// Blend and apply movement velocity and mode snapshots
	if (bSnapshotAnimationVariables)
	{
		const FAnimationSnapshot& PreviousSnapshot = AnimationRecentHistory[PreviousIndex];
		const FAnimationSnapshot& NextSnapshot = AnimationRecentHistory[LatestSnapshotIndex];
		ApplySnapshot(
			BlendSnapshots(PreviousSnapshot, NextSnapshot,
			               TimeSinceSnapshotsChanged / NextSnapshot.TimeSinceLastSnapshot),
			true /*bApplyTimeDilationToVelocity*/);
	}
}


FTransformAndVelocitySnapshot URewindComponent::BlendSnapshots(
	const FTransformAndVelocitySnapshot& A,
	const FTransformAndVelocitySnapshot& B,
	float Alpha, bool bForVisual)
{
	Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
	FTransformAndVelocitySnapshot BlendedSnapshot;
	BlendedSnapshot.TimeStamp = FMath::Lerp(A.TimeStamp, B.TimeStamp, Alpha);
	if (BlendedSnapshot.TimeStamp < StopRewindAt && !bForVisual)
	{
		Alpha = (StopRewindAt - A.TimeStamp) / (B.TimeStamp - A.TimeStamp);
		BlendedSnapshot.TimeStamp = StopRewindAt;
	}
	
	BlendedSnapshot.Transform.Blend(A.Transform, B.Transform, Alpha);
	BlendedSnapshot.Velocity = FMath::Lerp(A.Velocity, B.Velocity, Alpha);
	BlendedSnapshot.AngularVelocity = FMath::Lerp(A.AngularVelocity, B.AngularVelocity, Alpha);
	return BlendedSnapshot;
}


FAnimationSnapshot URewindComponent::BlendSnapshots(
	const FAnimationSnapshot& A,
	const FAnimationSnapshot& B,
	float Alpha)
{
	Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
	FAnimationSnapshot BlendedSnapshot;
	BlendedSnapshot.TimeStamp = FMath::Lerp(A.TimeStamp, B.TimeStamp, Alpha);
	if (BlendedSnapshot.TimeStamp < StopRewindAt)
	{
		Alpha = (StopRewindAt - A.TimeStamp) / (B.TimeStamp - A.TimeStamp);
		BlendedSnapshot.TimeStamp = StopRewindAt;
	}
	
	BlendedSnapshot.MovementVelocity = FMath::Lerp(A.MovementVelocity, B.MovementVelocity, Alpha);
	BlendedSnapshot.MovementMode = Alpha < 0.5f ? A.MovementMode : B.MovementMode;
	return BlendedSnapshot;
}

void URewindComponent::ApplySnapshot(const FTransformAndVelocitySnapshot& Snapshot, bool bApplyPhysics)
{
	GetOwner()->SetActorTransform(Snapshot.Transform);
	LocalTimeFromStart = Snapshot.TimeStamp;
	if (OwnerRootComponent && bApplyPhysics)
	{
		OwnerRootComponent->SetPhysicsLinearVelocity(Snapshot.Velocity);
		OwnerRootComponent->SetPhysicsAngularVelocityInRadians(Snapshot.AngularVelocity);
	}
}

void URewindComponent::ApplySnapshot(const FAnimationSnapshot& Snapshot, bool bApplyTimeDilationToVelocity)
{
	if (OwnerMovementComponent)
	{
		// Time dilation did not apply properly, probably because of the relatively extreme values 
		// after combined time dilation with rewind speed. So we instead adjust the global animation rate
		// of the character in the AC_TimeSlow Blueprint.
		OwnerMovementComponent->Velocity =
			bApplyTimeDilationToVelocity ? Snapshot.MovementVelocity /* * RewindSpeed */ : Snapshot.MovementVelocity;
		OwnerMovementComponent->SetMovementMode(Snapshot.MovementMode);
	}
}

void URewindComponent::ApplyEventRecordings(float Time)
{
	if (!IsValid(this) || !GetOwner())
	{
		return;
	}

	TArray<FRewindEvent*> EventsToApply = EventTimeline.FindAllDownTo(Time);

	for (FRewindEvent* Event : EventsToApply)
	{
		if (!Event || !Event->Function)
			continue;
		
		Event->Function();
	}
	EventTimeline.DeleteAllDownTo(Time);

	TArray<FRewindBPFloatEvent*> BPEventsToApply = BPEventTimeline.FindAllDownTo(Time);
	for (FRewindBPFloatEvent* BPEvent : BPEventsToApply)
	{
		if (BPEvent)
		{
			BPEvent->Delegate.ExecuteIfBound(BPEvent->StoredValue);
		}
	}
	BPEventTimeline.DeleteAllDownTo(Time);
}