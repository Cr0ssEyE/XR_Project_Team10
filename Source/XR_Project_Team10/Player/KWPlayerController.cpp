// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Player/KWPlayerController.h"
#include "Kismet/GameplayStatics.h"

AKWPlayerController::AKWPlayerController()
{
	
}

void AKWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::SetViewportMouseCaptureMode(this, EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
	FInputModeGameOnly GameOnlyInputMode;
	GameOnlyInputMode.SetConsumeCaptureMouseDown(false);
	SetShowMouseCursor(true);
	SetInputMode(GameOnlyInputMode);
}
