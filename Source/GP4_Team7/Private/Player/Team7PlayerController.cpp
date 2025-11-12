// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Team7PlayerController.h"

ATeam7PlayerController::ATeam7PlayerController()
{
	bReplicates = true;
}

void ATeam7PlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}
