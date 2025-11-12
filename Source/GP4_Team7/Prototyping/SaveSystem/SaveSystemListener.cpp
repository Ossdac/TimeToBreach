// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystemListener.h"

void USaveSystemListener::BroadcastLoadGame()
{
	OnLoadGame.Broadcast();
}
