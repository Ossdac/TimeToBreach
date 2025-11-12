// Galla

#include "AI/GP_AIController.h"
#include "AI/GP_AICharacter.h"
#include "AI/Components/GP_AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AI/Utils/GP_AISubsystem.h"

#include "BrainComponent.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "AI/Components/GP_CrowdFollowingComponent.h"

DEFINE_LOG_CATEGORY_STATIC(GP_AIControllerLog, All, All);

AGP_AIController::AGP_AIController(const FObjectInitializer& ObjectInitializer)
	//: Super(ObjectInitializer.SetDefaultSubobjectClass<UGP_CrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PerceptionComponent = CreateDefaultSubobject<UGP_AIPerceptionComponent>("AIPerceptionComponent");
	SetPerceptionComponent(*PerceptionComponent);
}

void AGP_AIController::SetAIState(EAIState State)
{
	//if(State == Blackboard->GetValueAsEnum(CurrentAIStateKeyName)) return;
	
	Blackboard->SetValueAsEnum(CurrentAIStateKeyName, State);
}

EAIState AGP_AIController::GetAIState() const
{
	EAIState State = EAIState(Blackboard->GetValueAsEnum(CurrentAIStateKeyName));
	return State;
}

void AGP_AIController::ResetStunnedState()
{
	UE_LOG(GP_AIControllerLog, Display, TEXT("ResetStunnedState"));
	SetAIState(EAIState::Passive);
	if (Blackboard->GetValueAsObject(TargetActorKeyName))
	{
		UGP_AIPerceptionComponent* GP_AIPerceptionComponent = Cast<UGP_AIPerceptionComponent>(PerceptionComponent);
		GP_AIPerceptionComponent->FindActiveSense(Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKeyName)));
	}
}

void AGP_AIController::ClearTarget()
{
	Blackboard->SetValueAsObject(TargetActorKeyName, nullptr);
}

void AGP_AIController::HandleSensedSight(AActor* TargetActor)
{
	EAIState State = GetAIState();
	switch (State)
	{
		case EAIState::Passive:
			Blackboard->SetValueAsObject(TargetActorKeyName, TargetActor);
			SetAIState(EAIState::Attacking);
			OnSightSensed.Broadcast();
			break;
		case EAIState::Attacking:
			OnSightSensed.Broadcast();
			break;
		case EAIState::Stunned:
			Blackboard->SetValueAsObject(TargetActorKeyName, TargetActor);
			OnSightSensed.Broadcast();
			break;
		case EAIState::Investigating:
			Blackboard->SetValueAsObject(TargetActorKeyName, TargetActor);
			SetAIState(EAIState::Attacking);
			OnSightSensed.Broadcast();
			break;
		case EAIState::Dead:
			break;
	}
}

void AGP_AIController::HandleSensedSound(FVector TargetLocation, AActor* TargetActor)
{
	EAIState State = GetAIState();
	switch (State)
	{
		case EAIState::Passive:
			Blackboard->SetValueAsVector(TargetLocationKeyName, TargetLocation);
			SetAIState(EAIState::Investigating);
			break;
		case EAIState::Attacking:
			break;
		case EAIState::Stunned:
			Blackboard->SetValueAsObject(TargetActorKeyName, TargetActor);
			break;
		case EAIState::Investigating:
			Blackboard->SetValueAsVector(TargetLocationKeyName, TargetLocation);
			SetAIState(EAIState::Investigating);
			break;
		case EAIState::Dead:
			break;
	}
}

void AGP_AIController::HandleSensedDamage(FVector TargetLocation, AActor* TargetActor)
{
	EAIState State = GetAIState();
	switch (State)
	{
		case EAIState::Passive:
			Blackboard->SetValueAsVector(TargetLocationKeyName, TargetLocation);
			SetAIState(EAIState::Investigating);
			break;
		case EAIState::Attacking:
			Blackboard->SetValueAsObject(TargetActorKeyName, TargetActor);
			break;
		case EAIState::Stunned:
			Blackboard->SetValueAsObject(TargetActorKeyName, TargetActor);
			break;
		case EAIState::Investigating:
			//Blackboard->SetValueAsObject(TargetActorKeyName, TargetActor);
			Blackboard->SetValueAsVector(TargetLocationKeyName, TargetLocation);
			SetAIState(EAIState::Investigating);
			break;
		case EAIState::Dead:
			break;
	}
}

void AGP_AIController::HandleSensedPrediction(FVector TargetLocation, AActor* TargetActor)
{
	ClearTarget();
	EAIState State = GetAIState();
	switch (State)
	{
		case EAIState::Passive:
			Blackboard->SetValueAsVector(TargetLocationKeyName, TargetLocation);
			SetAIState(EAIState::Investigating);
			break;
		case EAIState::Attacking:
			Blackboard->SetValueAsVector(TargetLocationKeyName, TargetLocation);
			SetAIState(EAIState::Investigating);
			break;
		case EAIState::Stunned:
			Blackboard->SetValueAsObject(TargetActorKeyName, TargetActor);
			break;
		case EAIState::Investigating:
			Blackboard->SetValueAsVector(TargetLocationKeyName, TargetLocation);
			SetAIState(EAIState::Investigating);
			break;
		case EAIState::Dead:
			break;
	}
}

ETeamAttitude::Type AGP_AIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	ETeamAttitude::Type Attitude;
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	if (OtherTeamAgent)
	{
		Attitude = FGenericTeamId::GetAttitude(GetGenericTeamId(), OtherTeamAgent->GetGenericTeamId());
	}
	else
	{
		Attitude = ETeamAttitude::Neutral;
	}
	return Attitude;
}

void AGP_AIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	Super::SetGenericTeamId(NewTeamID);

	IGenericTeamAgentInterface* ControlledAgent = Cast<IGenericTeamAgentInterface>(GetPawn());
	if (ControlledAgent)
	{
		ControlledAgent->SetGenericTeamId(NewTeamID);
	}
}

FGenericTeamId AGP_AIController::GetGenericTeamId() const
{
	const IGenericTeamAgentInterface* ControlledAgent = Cast<const IGenericTeamAgentInterface>(GetPawn());
	if (ControlledAgent)
	{
		return ControlledAgent->GetGenericTeamId();
	}

	return FGenericTeamId();
}

void AGP_AIController::SetReservedAttackTokens(int32 TokenAmount)
{
	if (TokenAmount < 0)
	{
		ReservedAttackTokens = 0;
	}
	else
	{
		ReservedAttackTokens = TokenAmount;
	}
}

void AGP_AIController::HandleDeath()
{
	SetAIState(EAIState::Dead);
	OnAICharacterDeath.Broadcast();
	BrainComponent->Cleanup();
}

void AGP_AIController::SetStunnedRemainedTime(float RemainedTime)
{
	Blackboard->SetValueAsFloat(StunnedRemainedTimeKeyName, RemainedTime);
	//UE_LOG(GP_AIControllerLog, Display, TEXT("RemainedTime = %f"), RemainedTime);
}

void AGP_AIController::BeginPlay()
{
	Super::BeginPlay();

	/*UGP_CrowdFollowingComponent* CrowdFollowingComponent = GetComponentByClass<UGP_CrowdFollowingComponent>();
	if (CrowdFollowingComponent)
	{
		CrowdFollowingComponent->SetCrowdSeparation(true);
		CrowdFollowingComponent->SetCrowdSeparationWeight(2.f);
		CrowdFollowingComponent->SetCrowdAvoidanceRangeMultiplier(1.f);
		CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
		CrowdFollowingComponent->SetCrowdAnticipateTurns(true);
	}*/
}

void AGP_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const AGP_AICharacter* AICharacter = Cast<AGP_AICharacter>(InPawn);
	if (!IsValid(AICharacter))
	{
		//UE_LOG(GP_AIControllerLog, Warning, TEXT("OnPossess: InPawn is not AGP_AICharacter"));
		return;
	}

	if (AICharacter->BehaviorTree)
	{
		RunBehaviorTree(AICharacter->BehaviorTree);
		auto Subsystem = GetWorld()->GetSubsystem<UGP_AISubsystem>();
		if (Subsystem)
		{
			Subsystem->RegisterAIListener(this);
		}

		//UE_LOG(GP_AIControllerLog, Display, TEXT("OnPossess: RunBehaviorTree"));
	}
}

FVector AGP_AIController::GetFocalPointOnActor(const AActor* Actor) const
{
	if (const ACharacter* TargetCharacter = Cast<ACharacter>(Actor))
	{
		if (const USkeletalMeshComponent* Mesh = TargetCharacter->GetMesh())
		{
			if (AimSocketName.IsNone() == false && Mesh->DoesSocketExist(AimSocketName))
			{
				return Mesh->GetSocketLocation(AimSocketName);
			}
		}

		const float Z = TargetCharacter->GetCapsuleComponent() ? TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.8f : 80.f;
		return TargetCharacter->GetActorLocation() + FVector(0, 0, Z);
	}

	return Actor ? Actor->GetActorLocation() : FVector::ZeroVector;
}

