// Galla


#include "AI/Utils/GP_SimpleProjectileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(GP_SimpleProjectileLog, All, All);

AGP_SimpleProjectileActor::AGP_SimpleProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponent);

	/*ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(CollisionComponent);*/

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->bRotationFollowsVelocity = true;
}

void AGP_SimpleProjectileActor::SetShotDirection(const FVector& Direction)
{
	ShotDirection = Direction;
	if (MovementComponent)
	{
		MovementComponent->Velocity = ShotDirection.GetSafeNormal() * MovementComponent->InitialSpeed;

		//UE_LOG(GP_SimpleProjectileLog, Warning, TEXT("SetShotDirection: Dir=%s | Velocity=%s"),	*ShotDirection.ToString(), *MovementComponent->Velocity.ToString());

		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + MovementComponent->Velocity.GetSafeNormal() * 200.f,	20.f, FColor::Cyan, false, 3.f, 0, 2.f);
	}
}

void AGP_SimpleProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	check(MovementComponent);
	check(CollisionComponent);

	//UE_LOG(GP_SimpleProjectileLog, Display, TEXT("BeginPlay: Forward=%s | Velocity=%s"), *GetActorForwardVector().ToString(), *MovementComponent->Velocity.ToString());

	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AGP_SimpleProjectileActor::OnProjectileHit);

	SetLifeSpan(LifeSeconds);
}


FVector AGP_SimpleProjectileActor::GetProjectileMovementDirection()
{
	const auto ShotDirectionNormal = ShotDirection.GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), ShotDirectionNormal));

	float Degrees = FMath::RadiansToDegrees(AngleBetween);
	FVector ProjectileDirection;
	return ProjectileDirection;
}

void AGP_SimpleProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;

	MovementComponent->StopMovementImmediately();

	// TakeDamage from BI_Damageable instead
	UGameplayStatics::ApplyRadialDamage(GetWorld(), //
		DamageAmount, //
		GetActorLocation(), //
		DamageRadius, //
		UDamageType::StaticClass(), //
		{ GetOwner() }, //
		this, //
		GetController(), //
		DoFullDamage);

	//DrawDebugSphere(GetWorld(),GetActorLocation(), DamageRadius, 24, FColor::Magenta, false, 5.0f);

	//Destroy();
	Deactivate();
}

AController* AGP_SimpleProjectileActor::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
