// Galla


#include "AI/Components/GP_BotMeleeAttackComponent.h"
#include "Components/DecalComponent.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY_STATIC(GP_BotMeleeAttackComponentLog, All, All);

UGP_BotMeleeAttackComponent::UGP_BotMeleeAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGP_BotMeleeAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()) return;

	USkeletalMeshComponent* SkeletalMeshComponent = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	if (!SkeletalMeshComponent) return;

	float OffsetZ = SkeletalMeshComponent->GetRelativeLocation().Z + 10.f;
	//UE_LOG(GP_BotMeleeAttackComponentLog, Display, TEXT("BeginPlay ExplosionEffectScaleMultiplier = %f"), ExplosionEffectScaleMultiplier);

	//InnerRingDecal = CreateDefaultSubobject<UDecalComponent>("InnerRingDecal");
	if (!InnerRingDecal)
	{
		InnerRingDecal = NewObject<UDecalComponent>(GetOwner(), "InnerRingDecal");
		InnerRingDecal->SetupAttachment(GetOwner()->GetRootComponent());
		InnerRingDecal->bHiddenInGame = true;
		InnerRingDecal->DecalSize = FVector(128.f, 256.f, 256.f);
		InnerRingDecal->SetDecalMaterial(ExplosionDecalMaterial);
		InnerRingDecal->RegisterComponent();
	}
	if (InnerRingDecal)
	{
		InnerRingDecal->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		InnerRingDecal->SetRelativeLocation(FVector(0.f, 0.f, -OffsetZ));
		InnerRingDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
		InnerRingDecal->SetRelativeScale3D(FVector(0.5f, 1.f * ExplosionEffectScaleMultiplier, 1.f * ExplosionEffectScaleMultiplier));
	}

	//OuterRingDecal = CreateDefaultSubobject<UDecalComponent>("OuterRingDecal");
	if (!OuterRingDecal)
	{
		OuterRingDecal = NewObject<UDecalComponent>(GetOwner(), "OuterRingDecal");
		OuterRingDecal->SetupAttachment(GetOwner()->GetRootComponent());
		OuterRingDecal->bHiddenInGame = true;
		OuterRingDecal->DecalSize = FVector(128.f, 256.f, 256.f);
		OuterRingDecal->SetDecalMaterial(ExplosionDecalMaterial);
		OuterRingDecal->RegisterComponent();
	}
	if (OuterRingDecal)
	{
		OuterRingDecal->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		OuterRingDecal->SetRelativeLocation(FVector(0.f, 0.f, -OffsetZ));
		OuterRingDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
		OuterRingDecal->SetRelativeScale3D(FVector(0.5f, 1.f * ExplosionEffectScaleMultiplier, 1.f * ExplosionEffectScaleMultiplier));
	}
}

void UGP_BotMeleeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bOnMeleeAttack)
	{
		ElapsedTime += DeltaTime * TimeDilation;

		float Alpha = ElapsedTime / Cooldown;
		float Value = FMath::Lerp(0.f, 0.5f, Alpha);

		if (InnerRingDecalMaterialInstance)
		{
			InnerRingDecalMaterialInstance->SetScalarParameterValue(TEXT("OuterRing"), Value);
		}

		if (ElapsedTime >= Cooldown)
		{
			ResetCooldown();
		}
	}
}

bool UGP_BotMeleeAttackComponent::CanPerformMeleeAttack_Internal() const
{
	return !bOnMeleeAttack && !bOnCooldownAfterMeleeAttack;
}

bool UGP_BotMeleeAttackComponent::IsBotInExplotionMode() const
{
	return bOnMeleeAttack;
}

void UGP_BotMeleeAttackComponent::PerformMeleeAttack_Implementation(AActor* InstigatorActor, AActor* Target)
{
	if (!CanPerformMeleeAttack_Internal() || !Target) return;
	//UE_LOG(GP_BotMeleeAttackComponentLog, Display, TEXT("PerformMeleeAttack"));

	if (InnerRingDecal && OuterRingDecal)
	{
		bOnMeleeAttack = true;
		if (!InnerRingDecalMaterialInstance)
		{
			UMaterialInterface* DecalMaterial = InnerRingDecal->GetDecalMaterial();
			if (DecalMaterial)
			{
				InnerRingDecalMaterialInstance = UMaterialInstanceDynamic::Create(DecalMaterial, this);
				InnerRingDecal->SetDecalMaterial(InnerRingDecalMaterialInstance);
			}
		}

		if (!OuterRingDecalMaterialInstance)
		{
			UMaterialInterface* DecalMaterial = OuterRingDecal->GetDecalMaterial();
			if (DecalMaterial)
			{
				OuterRingDecalMaterialInstance = UMaterialInstanceDynamic::Create(DecalMaterial, this);
				OuterRingDecal->SetDecalMaterial(OuterRingDecalMaterialInstance);
				OuterRingDecalMaterialInstance->SetScalarParameterValue(TEXT("FillValue"), 0.49f);
				OuterRingDecalMaterialInstance->SetScalarParameterValue(TEXT("OuterRing"), 0.5f);
			}
		}

		InnerRingDecal->bHiddenInGame = false;
		OuterRingDecal->bHiddenInGame = false;
	}

	//bOnCooldownAfterMeleeAttack = true;
	//GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UGP_BotMeleeAttackComponent::ResetCooldown, Cooldown, false);
}

void UGP_BotMeleeAttackComponent::StopPerformMeleeAttack()
{
	ResetCooldown();
}

void UGP_BotMeleeAttackComponent::ApplyMeleeAttackDamage_Implementation(AActor* Target, AActor* Instigator)
{
}

void UGP_BotMeleeAttackComponent::Explode_Implementation()
{
}

void UGP_BotMeleeAttackComponent::ResetCooldown()
{
	bOnCooldownAfterMeleeAttack = false;
	bIsDamageDone = false;
	bOnMeleeAttack = false;
	//GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);

	Explode();

	//UE_LOG(GP_BotMeleeAttackComponentLog, Display, TEXT("ResetCooldown"));
	OnMeleeAttackFinished.Broadcast();
}