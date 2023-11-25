// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "KWSaveSettingOption.generated.h"

UENUM()
enum class EResolutionTypes : uint8
{
	SD UMETA(DisplayName = "800x450"),
	HD UMETA(DisplayName = "1280x720"),
	HDP UMETA(DisplayName = "1600x900"),
	FHD UMETA(DisplayName = "1920x1080")
};

/**
 * 
 */

UCLASS()
class XR_PROJECT_TEAM10_API UKWSaveSettingOption : public USaveGame
{
	GENERATED_BODY()
	
public:
	UKWSaveSettingOption();

	UPROPERTY(EditDefaultsOnly, Category = SettingOption)
	FString SaveFileName;

	// SoundOption
public:
	UPROPERTY(EditDefaultsOnly, Category = SettingOption)
	float MasterSoundVolumeSliderValue;

	UPROPERTY(EditDefaultsOnly, Category = SettingOption)
	float BGMSoundVolumeSliderValue;

	UPROPERTY(EditDefaultsOnly, Category = SettingOption)
	float SFXSoundVolumeSliderValue;
	
	// ResolutionOption
public:
	UPROPERTY(EditDefaultsOnly, Category = SettingOption)
	EResolutionTypes ResolutionType;

	UPROPERTY(EditDefaultsOnly, Category = SettingOption)
	uint8 bIsFullScreenActivate : 1;
	
};
