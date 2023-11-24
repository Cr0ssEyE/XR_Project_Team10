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
	CloseBtn->OnClicked.AddDynamic(this, &UKWSettingWidget::CloseSettingWidget);
	
	for (int i = 0; i <= static_cast<int>(EResolutionTypes::FHD); i++)
	{
		ResolutionEnumMap.Add(StaticEnum<EResolutionTypes>()->GetDisplayNameTextByIndex(i).ToString(), static_cast<EResolutionTypes>(i));
	}
	LoadSettingData(GetWorld()->GetGameInstanceChecked<UKWGameInstance>()->GetSaveSettingOption());
}

void UKWSettingWidget::SaveSettingData(UKWSaveSettingOption* SettingOption)
{
	SettingOption->MasterSoundVolumeSliderValue = MasterSoundVolumeSlider->GetValue();
	SettingOption->BGMSoundVolumeSliderValue = BGMSoundVolumeSlider->GetValue();
	SettingOption->SFXSoundVolumeSliderValue = SFXSoundVolumeSlider->GetValue();
	SettingOption->ResolutionType = ResolutionTypes;
	SettingOption->bIsFullScreenActivate = bIsFullScreenActivate;

	UGameplayStatics::SaveGameToSlot(SettingOption, SettingOption->SaveFileName, 0);
}

void UKWSettingWidget::LoadSettingData(UKWSaveSettingOption* SettingOption)
{
	MasterSoundVolumeSlider->SetValue(SettingOption->MasterSoundVolumeSliderValue);
	BGMSoundVolumeSlider->SetValue(SettingOption->BGMSoundVolumeSliderValue);
	SFXSoundVolumeSlider->SetValue(SettingOption->SFXSoundVolumeSliderValue);
	ResolutionTypes = SettingOption->ResolutionType;
	ResolutionComboBox->SetSelectedOption(*ResolutionEnumMap.FindKey(ResolutionTypes));
	bIsFullScreenActivate = SettingOption->bIsFullScreenActivate;
	
	MasterSoundVolumeSlider->OnValueChanged.Broadcast(MasterSoundVolumeSlider->GetValue());
	BGMSoundVolumeSlider->OnValueChanged.Broadcast(BGMSoundVolumeSlider->GetValue());
	SFXSoundVolumeSlider->OnValueChanged.Broadcast(SFXSoundVolumeSlider->GetValue());
	
	ESelectInfo::Type Info = ESelectInfo::OnKeyPress;
	ResolutionComboBox->OnSelectionChanged.Broadcast(ResolutionComboBox->GetSelectedOption(), Info);
	bIsFullScreenActivate == true ? ApplyFullScreen() : ApplyWindowScreen();
}

void UKWSettingWidget::ApplyResolutionType(FString ResolutionName, ESelectInfo::Type Info)
{
	if(ResolutionName.IsEmpty())
	{
		return;
	}
	ResolutionTypes = ResolutionEnumMap[ResolutionName];
	switch (ResolutionTypes)
	{
	case EResolutionTypes::SD:
		GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(800, 450));
		break;
	case EResolutionTypes::HD:
		GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(1280, 720));
		break;
	case EResolutionTypes::HDP:
		GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(1600, 900));
		break;
	case EResolutionTypes::FHD:
		GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(1920, 1080));
		break;
	default:
		checkNoEntry();
	}
	GEngine->GetGameUserSettings()->ApplySettings(false);
}

void UKWSettingWidget::ApplyFullScreen()
{
	bIsFullScreenActivate = true;
	GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	GEngine->GetGameUserSettings()->ApplySettings(false);
}

void UKWSettingWidget::ApplyWindowScreen()
{
	bIsFullScreenActivate = false;
	GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
	GEngine->GetGameUserSettings()->ApplySettings(false);
}
