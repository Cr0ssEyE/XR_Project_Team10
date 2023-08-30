// Copyright Epic Games, Inc. All Rights Reserved.

#include "XR_Project_Team10GameMode.h"
#include "XR_Project_Team10PlayerController.h"
#include "XR_Project_Team10Character.h"
#include "UObject/ConstructorHelpers.h"

AXR_Project_Team10GameMode::AXR_Project_Team10GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AXR_Project_Team10PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}