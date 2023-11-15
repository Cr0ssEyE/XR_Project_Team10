// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWSettingWidget.h"

#include "Components/Button.h"
#include "Components/ComboBoxKey.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Types/SlateEnums.h"
#include "XR_Project_Team10/Game/KWGameInstance.h"

void UKWSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MasterSoundVolumeSlider->OnValueChanged.AddDynamic(this, &UKWSettingWidget::ApplyMasterSliderValue);
	BGMSoundVolumeSlider->OnValueChanged.AddDynamic(this, &UKWSettingWidget::ApplyBGMSliderValue);
	SFXSoundVolumeSlider->OnValueChanged.AddDynamic(this, &UKWSettingWidget::ApplySFXSliderValue);

	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UKWSettingWidget::ApplyResolutionType);
	UseFullScreenBtn->OnClicked.AddDynamic(this, &UKWSettingWidget::ApplyFullScreen);
	UseWindowBtn->OnClicked.AddDynamic(this, &UKWSettingWidget::ApplyWindowScreen);
	
	for (int i = 0; i <= static_cast<int>(EResolutionTypes::FHD); i++)
	{
		ResolutionEnumMap.Add(StaticEnum<EResolutionTypes>()->GetDisplayNameTextByIndex(i).ToString(), static_cast<EResolutionTypes>(i));
	}
	LoadSettingData(GetWorld()->GetGameInstanceChecked<UKWGameInstance>()->GetSaveSettingOption());
}

void UKWSettingWidget::SaveSettingData(UKWSaveSettingOption* SettingOption)
{
	
}

void UKWSettingWidget::LoadSettingData(UKWSaveSettingOption* SettingOption)
{
	
}

void UKWSettingWidget::ApplyResolutionType(FString ResolutionName, ESelectInfo::Type Info)
{
	ResolutionTypes = ResolutionEnumMap[ResolutionName];
	switch (ResolutionTypes)
	{
	case EResolutionTypes::SD:
		GEngine->GameUserSettings->SetScreenResolution(FIntPoint(800, 450));
		break;
	case EResolutionTypes::HD:
		GEngine->GameUserSettings->SetScreenResolution(FIntPoint(1280, 720));
		break;
	case EResolutionTypes::HDP:
		GEngine->GameUserSettings->SetScreenResolution(FIntPoint(1600, 900));
		break;
	case EResolutionTypes::FHD:
		GEngine->GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
		break;
		default:
			checkNoEntry();
	}
}

void UKWSettingWidget::ApplyFullScreen()
{
	bIsFullScreenActivate = true;
	GEngine->GameUserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
}

void UKWSettingWidget::ApplyWindowScreen()
{
	bIsFullScreenActivate = false;
	GEngine->GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
}
