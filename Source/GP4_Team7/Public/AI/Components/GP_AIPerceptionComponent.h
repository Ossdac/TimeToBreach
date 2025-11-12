// Galla

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "GP_AIPerceptionComponent.generated.h"

class UGP_AISubsystem;

UCLASS()
class GP4_TEAM7_API UGP_AIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// can be deleted
	AActor* GetTargetEnemy() const;

	void FindActiveSense(AActor* Actor);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI|Prediction")
	float PredictionTickInterval = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Prediction")
	float PredictionLoopMaxDuration = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Prediction")
	float PredictionTime = 1.0f;

	UFUNCTION()
	void OnPerceptionTargetUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnPerceptionTargetForgotten(AActor* Actor);

private:

	FTimerHandle PredictionLoopTimer;
	float PredictionLoopStopAtTime = 0.f;
	TWeakObjectPtr<AActor> PredictedActor;

	UPROPERTY()
	UGP_AISubsystem* AISubsytem;

	UFUNCTION()
	void HandleDeath();

	void StartPredictionLoop(AActor* LostActor);
	void StopPredictionLoop();
	void TickPredictionLoop();
};
