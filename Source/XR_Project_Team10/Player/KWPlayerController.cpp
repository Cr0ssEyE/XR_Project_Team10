// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Player/KWPlayerController.h"

AKWPlayerController::AKWPlayerController()
{
	
}

void AKWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameOnly GameOnlyInputMode;
	SetShowMouseCursor(true);
	SetInputMode(GameOnlyInputMode);
}
