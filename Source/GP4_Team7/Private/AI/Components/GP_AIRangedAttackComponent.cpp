// Galla


#include "AI/Components/GP_AIRangedAttackComponent.h"
#include "GameFramework/Character.h"
#include "AI/Utils/GP_SimpleProjectileActor.h"
#include "DrawDebugHelpers.h"
#include "ObjectPool/ObjectPool.h"
#include "Components/ActorComponent.h"
#include "../../../GameModes/GameplayGamemode.h"

DEFINE_LOG_CATEGORY_STATIC(GP_AIRangedAttackComponentLog, All, All);

UGP_AIRangedAttackComponent::UGP_AIRangedAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGP_AIRangedAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	/*if (!ProjectileClass && !GetOwner()) return;

	if (!ProjectilePool)
	{
		ProjectilePool = CreateDefaultSubobject<UObjectPool>(TEXT("ProjectilePool"));
		ProjectilePool->SetupAttachment(GetOwner()->GetRootComponent());
		ProjectilePool->PooledObjectClass = ProjectileClass;
		ProjectilePool->PoolSize = PoolSize;
		ProjectilePool->PooledObjectLifeSpan = ProjectileLifeSpan;
		ProjectilePool->InitialSpawnedObjectsLocation = FVector(0, 0, -100000);
		ProjectilePool->RegisterComponent();
		UE_LOG(GP_AIRangedAttackComponentLog, Warning, TEXT("ProjectilePool"));
	}
	if (ProjectilePool)
	{

	}*/



	//if (GetWorld() && GetWorld()->GetAuthGameMode())
	//{
	//	AGameplayGamemode* GM = Cast<AGameplayGamemode>(GetWorld()->GetAuthGameMode());
	//	if (GM)
	//	{
	//		GameMode = GM;


	//		GameMode->OnStartGlobalNormalTime.AddDynamic(this, &UGP_AIRangedAttackComponent::OnStartGlobalNormalTime);
	//		GameMode->OnStopGlobalNormalTime.AddDynamic(this, &UGP_AIRangedAttackComponent::OnStopGlobalNormalTime);
	//		//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("Cast GameMode"));
	//	}
	//	else
	//	{
	//		//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT(" no Cast GameMode"));
	//	}
	//}
	//else
	//{
	//	//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT(" no"));
	//}
}

void UGP_AIRangedAttackComponent::MakeShot(FName SocketName)
{
	if (!ProjectileClass) return;

	const FVector MuzzleLocation = GetMuzzleWorldLocation(SocketName);
	const FRotator MuzzleRotation = GetMuzzleWorldRotation(SocketName);

	// Eyes location fo aiming
	FVector EyeLocation;
	FRotator EyeRotation;

	const ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (Char)
	{
		Char->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	}
	else
	{
		EyeLocation = MuzzleLocation;
		EyeRotation = GetOwner()->GetActorRotation();
	}

	//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("Eye: Location=%s Rotation=%s | Muzzle: Location=%s Rotation=%s"), *EyeLocation.ToString(), *EyeRotation.ToString(), *MuzzleLocation.ToString(), *MuzzleRotation.ToString());

	// Direction from eyes towards
	const FVector TraceEnd = EyeLocation + EyeRotation.Vector() * TraceMaxDistance;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ProjectileAim), true, GetOwner());
	Params.AddIgnoredActor(GetOwner());

	FVector AimPoint = TraceEnd;
	if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, Params))
	{
		AimPoint = Hit.ImpactPoint;
	}

	//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("AimPoint=%s"), *AimPoint.ToString());

	// line of sight
	//DrawDebugLine(GetWorld(), EyeLocation, AimPoint, FColor::Red, false, 3.f, 0, 1.f);
	// line from muzzle to AimPoint
	//DrawDebugLine(GetWorld(), MuzzleLocation, AimPoint, FColor::Blue, false, 3.f, 0, 1.f);

	// Direction from muzzle to AimPoint
	const FVector ShotDirection = (AimPoint - MuzzleLocation).GetSafeNormal();
	const FRotator SpawnRotation = ShotDirection.Rotation();

	//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("ShotDirection=%s | SpawnRotation=%s"), *ShotDirection.ToString(), *SpawnRotation.ToString());

	// spawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto* Projectile = GetWorld()->SpawnActor<AGP_SimpleProjectileActor>(ProjectileClass, MuzzleLocation, SpawnRotation, SpawnParams);

	/*if (!GameMode->ProjectilePool)
	{

		UE_LOG(GP_AIRangedAttackComponentLog, Warning, TEXT("!GameMode->ProjectilePool"));
		return;
	}
	APooledObject* Pooled = GameMode->ProjectilePool ? GameMode->ProjectilePool->SpawnPooledObjectWithDifferentLifeSpan(MuzzleLocation, SpawnRotation, 1.f) : nullptr;
	AGP_SimpleProjectileActor* Projectile = Cast<AGP_SimpleProjectileActor>(Pooled);*/
	if (!Projectile)
	{ 
		//UE_LOG(GP_AIRangedAttackComponentLog, Warning, TEXT("NO PROJETCILE"));
		StopPerformRangedAttack(); 
		return; 
	}
	else
	{
		//UE_LOG(GP_AIRangedAttackComponentLog, Warning, TEXT("PROJETCILE"));
	}

	Projectile->SetOwner(GetOwner());
	Projectile->SetShotDirection(ShotDirection);

	//DrawDebugDirectionalArrow(GetWorld(), MuzzleLocation, MuzzleLocation + ShotDirection * 200.f, 20.f, FColor::Green, false, 3.f, 0, 2.f);

	SpawnMuzzleFlashEffect();
}

void UGP_AIRangedAttackComponent::SpawnMuzzleFlashEffect()
{
}

void UGP_AIRangedAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bOnCooldownAfterRangedAttack)
	{
		if (ElapsedCooldownTime >= Cooldown)
		{
			ResetCooldown();
		}
		else
		{
			ElapsedCooldownTime += DeltaTime * TimeDilation;

			//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("ElapsedCooldownTime = %f | Cooldown = %f | TimeDilation = %f"), ElapsedCooldownTime, Cooldown, TimeDilation);
		}
	}
}

bool UGP_AIRangedAttackComponent::CanPerformRangedAttack() const
{
	return !bOnRangedAttack && !bOnCooldownAfterRangedAttack && ProjectileClass != nullptr;
}

void UGP_AIRangedAttackComponent::PerformRangedAttack_Implementation(AActor* InstigatorActor, AActor* Target)
{
	if (!CanPerformRangedAttack() || !Target) return;

	//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("PerformRangedAttack"));

	bOnRangedAttack = true;
	MakeShot(MuzzleSocketName);

	//bOnCooldownAfterRangedAttack = true;
	//GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UGP_AIRangedAttackComponent::ResetCooldown, Cooldown, false);
}

void UGP_AIRangedAttackComponent::StopPerformRangedAttack()
{
	bOnCooldownAfterRangedAttack = true;
	bOnRangedAttack = false;
	//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("StopPerformRangedAttack"));
	//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("bOnRangedAttack = %s"), bOnRangedAttack ? TEXT("TRUE") : TEXT("FALSE"));
	//GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	OnRangedAttackFinished.Broadcast();
}

FOnAIRangedAttackFinishedSignature& UGP_AIRangedAttackComponent::GetAIRangedAttackFinishedDelegate()
{
	return OnRangedAttackFinished;
}

void UGP_AIRangedAttackComponent::OnStartGlobalNormalTime()
{
	TimeDilation = 1.0f;
	//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("OnStartGlobalNormalTime TimeDilation = %f"), TimeDilation);
}

void UGP_AIRangedAttackComponent::OnStopGlobalNormalTime(float Time)
{
	TimeDilation = Time;
	//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("OnStopGlobalNormalTime TimeDilation = %f"), TimeDilation);
}

void UGP_AIRangedAttackComponent::ResetCooldown()
{
	if (bOnCooldownAfterRangedAttack)
	{
		bOnCooldownAfterRangedAttack = false;
	}

	ElapsedCooldownTime = 0.f;
	// GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);

	//UE_LOG(GP_AIRangedAttackComponentLog, Display, TEXT("ResetCooldown"));
	//OnRangedAttackFinished.Broadcast();
}

FVector UGP_AIRangedAttackComponent::GetMuzzleWorldLocation(FName SocketName) const
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (!Character) return FVector::ZeroVector;

	return Character->GetMesh()->GetSocketLocation(SocketName);
}

FRotator UGP_AIRangedAttackComponent::GetMuzzleWorldRotation(FName SocketName) const
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (!Character) return FRotator::ZeroRotator;

	return Character->GetMesh()->GetSocketRotation(SocketName);
}