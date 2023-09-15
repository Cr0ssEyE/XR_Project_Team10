// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Game/KWGameModeBase.h"

#include "XR_Project_Team10/Util/PPConstructorHelper.h"

AKWGameModeBase::AKWGameModeBase()
{
	DefaultPawnClass = FPPConstructorHelper::FindAndGetClass<APawn>(TEXT("/Script/CoreUObject.Class'/Script/XR_Project_Team10.KWPlayerCharacter'"));
	PlayerControllerClass = FPPConstructorHelper::FindAndGetClass<APlayerController>(TEXT("/Script/CoreUObject.Class'/Script/XR_Project_Team10.KWPlayerController'"));
}
