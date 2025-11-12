// Galla

#include "AI/GP_AICharacter.h"
#include "AI/GP_AIController.h"
//#include "GameFramework/CharacterMovementComponent.h" //no need
#include "AI/Components/GP_AICharacterMovementComponent.h"
#include "AI/Utils/GP_PatrolRouteActor.h"
#include "AI/Utils/GP_AttackTokenSubsystem.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Prototyping/SaveSystem/EnemySaveStateSnapshotComponent.h"
#include "Prototyping/SaveSystem/LogSaveCategory.h"
#include "Navigation/CrowdManager.h"

#include "../../GameModes/GameplayGamemode.h"
#include "BehaviorTree/BlackboardComponent.h"

//#include "AI/Components/GP_AIRangedAttackComponent.h"

DEFINE_LOG_CATEGORY_STATIC(GP_AICharacterLog, All, All);

AGP_AICharacter:: AGP_AICharacter(const FObjectInitializer& ObjInit):
	Super(ObjInit.SetDefaultSubobjectClass<UGP_AICharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; //PlacedInWorldOrSpawned  Disabled
	AIControllerClass = AGP_AIController::StaticClass();

	bUseControllerRotationYaw = false;
	/*if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}*/

	// Added by Martin M
	RegisterSaveComponent = CreateDefaultSubobject<URegisterSaveComponent>(TEXT("RegisterSaveComponent"));
	EnemySaveStateSnapshotComponent = CreateDefaultSubobject<UEnemySaveStateSnapshotComponent>(TEXT("EnemySaveStateSnapshotComponent"));
}

void AGP_AICharacter::BeginPlay()
{
	Super::BeginPlay();

	/*auto CrowdManager = UCrowdManager::GetCurrent(this);
	if (CrowdManager)
	{
		CrowdManager->RegisterAgent(this);
	}*/

	if (!GetWorld()) return;

	AGameplayGamemode* GameplayGameMode = Cast<AGameplayGamemode>(GetWorld()->GetAuthGameMode());
	if (!GameplayGameMode) return;

	GameplayGameMode->OnStartGlobalNormalTime.AddDynamic(this, &AGP_AICharacter::OnStartGlobalNormalTime);
	GameplayGameMode->OnStopGlobalNormalTime.AddDynamic(this, &AGP_AICharacter::OnStopGlobalNormalTime);

	RemainedStunnedCooldownTime = StunnedTime;
}

void AGP_AICharacter::BeginDestroy()
{
	Super::BeginDestroy();

	/*auto CrowdManager = UCrowdManager::GetCurrent(this);
	if (CrowdManager)
	{
		CrowdManager->UnregisterAgent(this);
	}*/
}

//void AGP_AICharacter::SetMovementSpeed_Implementation(ESpeedState SpeedState)
//{
//
//}

void AGP_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AIController)
	{
		//UE_LOG(GP_AICharacterLog, Display, TEXT("Tick AIController"));
		if (AIController->GetAIState() == EAIState::Stunned)
		{
			//UE_LOG(GP_AICharacterLog, Display, TEXT("GetAIState() == EAIState::Stunned"));
			if (RemainedStunnedCooldownTime <= 0.f)
			{
				AIController->ResetStunnedState();
				//AIController->SetAIState(EAIState::Passive);
				RemainedStunnedCooldownTime = StunnedTime;
			}
			else
			{
				RemainedStunnedCooldownTime -= DeltaTime * TimeDilation;
				AIController->SetStunnedRemainedTime(RemainedStunnedCooldownTime);
			}
		}
	}
	else
	{
		AIController = Cast<AGP_AIController>(Controller);
	}
}

void AGP_AICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGP_AICharacter::HandleDeath()
{
	//const auto AIController = Cast<AGP_AIController>(Controller);
	if (!AIController) return;

	UGP_AttackTokenSubsystem* AttackTokenSubsystem = GetWorld()->GetSubsystem<UGP_AttackTokenSubsystem>();
	if (AttackTokenSubsystem)
	{
		AttackTokenSubsystem->ReleaseToken(AIController, 1);
	}
	AIController->HandleDeath();
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //ECC_GameTraceChannel1
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

//FVector AGP_AICharacter::GetCrowdAgentLocation() const
//{
//	return GetActorLocation();
//}
//
//FVector AGP_AICharacter::GetCrowdAgentVelocity() const
//{
//	return GetCharacterMovement()->GetVelocityForRVOConsideration();
//}
//
//void AGP_AICharacter::GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const
//{
//	CylinderRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
//	CylinderHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
//}
//
//float AGP_AICharacter::GetCrowdAgentMaxSpeed() const
//{
//	return GetCharacterMovement()->GetMaxSpeed();
//}


//bool AGP_AICharacter::CanRangedAttack_Implementation() const
//{
//	const auto* RangedAttackComponent = FindComponentByClass<UGP_AIRangedAttackComponent>();
//	return RangedAttackComponent && RangedAttackComponent->CanPerformRangedAttack();
//}

// Added by Martin M
void AGP_AICharacter::LoadFromSaveGame_Implementation(UMyGameSave* SaveGame, bool bIsLevelChanging)
{
	ISavable::LoadFromSaveGame_Implementation(SaveGame, bIsLevelChanging);

	if (const UWorld* World = GetWorld(); IsValid(World))
	{
		if (!World->IsGameWorld()) return;
		if (IsValid(SaveGame) && IsValid(RegisterSaveComponent))
		{			
			if (const int32 Identifier = RegisterSaveComponent->GetIdentifier())
			{
				SaveGame->LoadEnemySnapshot(this, Identifier);
			}
		}
	}
}

// Added by Martin M
void AGP_AICharacter::SaveToSaveGame_Implementation(UMyGameSave* SaveGame)
{
	ISavable::SaveToSaveGame_Implementation(SaveGame);
	if (IsValid(SaveGame) && IsValid(RegisterSaveComponent))
	{
		SaveGame->SaveEnemySnapshot(this, RegisterSaveComponent->GetIdentifier());
	}

}
void AGP_AICharacter::OnStartGlobalNormalTime()
{
	TimeDilation = 1.0f;
}

void AGP_AICharacter::OnStopGlobalNormalTime(float Time)
{
	TimeDilation = Time;
}