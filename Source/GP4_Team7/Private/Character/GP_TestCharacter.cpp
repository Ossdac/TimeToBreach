// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GP_TestCharacter.h"

// Sets default values
AGP_TestCharacter::AGP_TestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGP_TestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGP_TestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGP_TestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

