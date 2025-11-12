// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RegisterSaveComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_TEAM7_API URegisterSaveComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Transient)
	FString CachedName;

	UPROPERTY(VisibleAnywhere, Transient)
	int64 CachedIdentifier = -1;
	

public:
	// Sets default values for this component's properties
	URegisterSaveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
public:
	UFUNCTION(BlueprintCallable, Category="SaveGame")
	int32 GetIdentifier() const;

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	static int32 CalculateIdentifier(AActor* Actor);
};
