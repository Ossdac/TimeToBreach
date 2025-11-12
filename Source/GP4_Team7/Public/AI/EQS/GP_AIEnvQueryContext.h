// Galla

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "GP_AIEnvQueryContext.generated.h"

UCLASS()
class GP4_TEAM7_API UGP_AIEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

public:

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EQS")
	FName TargetActorKeyName = "TargetActor";
};
