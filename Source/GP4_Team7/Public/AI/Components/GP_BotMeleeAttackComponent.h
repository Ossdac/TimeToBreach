// Galla

#pragma once

#include "CoreMinimal.h"
#include "AI/Components/GP_AIMeleeAttackComponent.h"
#include "GP_BotMeleeAttackComponent.generated.h"

class UDecalComponent;
class UMaterialInstanceDynamic;
class UMaterialInstance;

UCLASS()
class GP4_TEAM7_API UGP_BotMeleeAttackComponent : public UGP_AIMeleeAttackComponent
{
	GENERATED_BODY()

public:

	UGP_BotMeleeAttackComponent();

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MeleeAttack")
	UDecalComponent* InnerRingDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MeleeAttack")
	UDecalComponent* OuterRingDecal;

	virtual bool CanPerformMeleeAttack_Internal() const override;

	UFUNCTION(BlueprintCallable, Category = "MeleeAttack")
	bool IsBotInExplotionMode() const;

	void PerformMeleeAttack_Implementation(AActor* InstigatorActor, AActor* Target) override;

	virtual void StopPerformMeleeAttack() override;

	void ApplyMeleeAttackDamage_Implementation(AActor* Target, AActor* Instigator) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MeleeAttack")
	void Explode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeAttack")
	float ExplosionEffectScaleMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeAttack")
	float ExplosionRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeAttack")
	bool DoFullDamage = false;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeAttack")
	UMaterialInstance* ExplosionDecalMaterial;

protected:

	virtual void ResetCooldown() override;

	float ElapsedTime = 0.f;

	UPROPERTY()
	UMaterialInstanceDynamic* InnerRingDecalMaterialInstance = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* OuterRingDecalMaterialInstance = nullptr;
	
};
