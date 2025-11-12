// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utils/GP_AISubsystem.h"
#include "AI/GP_AIController.h"
#include "AI/GP_AICharacter.h"

DEFINE_LOG_CATEGORY_STATIC(GP_AISubsystemLog, All, All);

void UGP_AISubsystem::RegisterAIListener(AGP_AIController* Controller)
{
	if (!Controller) return;

	if (!AIListeners.Contains(Controller))
	{
		AIListeners.Add(Controller, true);
		//UE_LOG(GP_AISubsystemLog, Display, TEXT("RegisterAIListener %s"), *Controller->GetName());
	}
}

void UGP_AISubsystem::UnregisterAIListener(AGP_AIController* Controller)
{
	if (!Controller) return;

	if (AIListeners.Contains(Controller))
	{
		//UE_LOG(GP_AISubsystemLog, Display, TEXT("UnregisterAIListener %s"), *Controller->GetName());
		if (AIListeners.Find(Controller))
		{
			AIListeners.Remove(Controller);
		}
		else
		{
			AIListeners.Remove(Controller);
			if (IsAllAIForgotCharacter())
			{
				OnAllAIForgotCharacter.Broadcast();
			}
		}

	}
}

bool UGP_AISubsystem::IsAllAIForgotCharacter()
{
	bool IsForgotten = true;
	for (const auto& Listener : AIListeners)
	{
		if (!Listener.Value)
		{
			IsForgotten = false;
			return IsForgotten;
		}
	}
	return IsForgotten;
}

void UGP_AISubsystem::SetIsCharacterForgotten(AGP_AIController* Controller, bool IsForgotten)
{
	if (!Controller) return;

	if (AIListeners.Contains(Controller))
	{
		//UE_LOG(GP_AISubsystemLog, Display, TEXT("AIController %s forgot : %s"), *Controller->GetName(), IsForgotten ? TEXT("TRUE") : TEXT("FALSE"));
		AIListeners[Controller] = IsForgotten;
	}

	if (IsForgotten && IsAllAIForgotCharacter())
	{
		OnAllAIForgotCharacter.Broadcast();
	}
}
