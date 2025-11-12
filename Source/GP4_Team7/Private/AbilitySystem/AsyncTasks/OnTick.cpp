// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/OnTick.h"


UOnTick::UOnTick(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

UOnTick* UOnTick::AbilityTaskOnTick(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UOnTick* MyObj = NewAbilityTask<UOnTick>(OwningAbility, TaskInstanceName);
	return MyObj;
}

void UOnTick::Activate()
{
	Super::Activate();
}

void UOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTick.Broadcast(DeltaTime);
	}
}
