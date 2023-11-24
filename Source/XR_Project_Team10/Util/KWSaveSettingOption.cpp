// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Util/KWSaveSettingOption.h"

UKWSaveSettingOption::UKWSaveSettingOption()
{
	SaveFileName = "SettingOptionsData";
	MasterSoundVolumeSliderValue = 0.5f;
	BGMSoundVolumeSliderValue = 0.5f;
	SFXSoundVolumeSliderValue = 0.5f;
	ResolutionType = EResolutionTypes::HDP;
	bIsFullScreenActivate = false;
}
