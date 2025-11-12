// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "GP_EQSContext_TargetLocation.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_EQSContext_TargetLocation : public UEnvQueryContext
{
	GENERATED_BODY()
public:

    virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EQS")
    FName TargetLocationKeyName = "TargetLocation";
};
