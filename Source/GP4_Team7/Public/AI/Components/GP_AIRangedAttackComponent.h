// Galla

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI/Interfaces/GP_RangedAttackInterface.h"
#include "GP_AIRangedAttackComponent.generated.h"

class AGP_SimpleProjectileActor;
class APooledObject;
class UObjectPool;
class AGameplayGamemode;

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API UGP_AIRangedAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UGP_AIRangedAttackComponent();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "RangedAttack")
	void MakeShot(FName SocketName);

	UFUNCTION(BlueprintCallable, Category = "RangedAttack")
	void SpawnMuzzleFlashEffect();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RangedAttack")
	float TraceMaxDistance = 1500.f;

	/*UPROPERTY()
	TObjectPtr<UObjectPool> ProjectilePool;

	UPROPERTY(EditAnywhere, Category = "RangedAttack|Pooling")
	int32 PoolSize = 20;

	UPROPERTY(EditAnywhere, Category = "RangedAttack|Pooling")
	float ProjectileLifeSpan = 5.f;*/

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangedAttack")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangedAttack")
	float Cooldown = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangedAttack")
	TSubclassOf<AGP_SimpleProjectileActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangedAttack")
	FName MuzzleSocketName = "MuzzleSocket";

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "RangedAttack")
	bool CanPerformRangedAttack() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RangedAttack")
	void PerformRangedAttack(AActor* InstigatorActor, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "RangedAttack")
	void StopPerformRangedAttack();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "RangedAttack")
	FOnAIRangedAttackFinishedSignature OnRangedAttackFinished;

	FOnAIRangedAttackFinishedSignature& GetAIRangedAttackFinishedDelegate();

protected:

	UFUNCTION()
	void OnStartGlobalNormalTime();

	UFUNCTION()
	void OnStopGlobalNormalTime(float Time);

	void ResetCooldown();

	UPROPERTY(BlueprintReadWrite, Category = "RangedAttack")
	bool bOnRangedAttack = false;

	bool bOnCooldownAfterRangedAttack = false;

	float ElapsedCooldownTime = 0.f;
	float TimeDilation = 1.0f;

	FTimerHandle CooldownTimerHandle;

	FVector GetMuzzleWorldLocation(FName SocketName) const;
	FRotator GetMuzzleWorldRotation(FName SocketName) const;

	AGameplayGamemode* GameMode = nullptr;
};
