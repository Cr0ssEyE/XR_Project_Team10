// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KWPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API AKWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKWPlayerController();

protected:
	virtual void BeginPlay() override;
	
private:
};
