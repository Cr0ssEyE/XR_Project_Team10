// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Util/KWSaveSettingOption.h"

UKWSaveSettingOption::UKWSaveSettingOption()
{
	SaveFileName = "SettingOptionsData";
	MasterSoundVolumeSliderValue = 0.5f;
	BGMSoundVolumeSliderValue = 0.5f;
	SFXSoundVolumeSliderValue = 0.5f;
	bMasterSoundToggle = false;
	bBGMSoundToggle = false;
	bSFXSoundToggle = false;
	DisplayBrightnessValue = 0.5f;
	DisplayVignettingValue = 0.0f;
	TextureQualityValue = 3;
	ShadowQualityValue = 3;
	AAType = 1;
	AAQualityValue = 3;
}