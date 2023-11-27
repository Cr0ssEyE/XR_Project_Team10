// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundClass.h"
#include "Components/ProgressBar.h"
#include "XR_Project_Team10/Game/KWGameInstance.h"
#include "XR_Project_Team10/Util/KWSaveSettingOption.h"
#include "KWSettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWSettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SaveSettingData(UKWSaveSettingOption* SettingOption);
	void LoadSettingData(UKWSaveSettingOption* SettingOption);
	
protected:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ApplyMasterSliderValue(const float Value) { MasterSoundClass->Properties.Volume = Value; MasterVolumeProgressBar->SetPercent(Value); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ApplyBGMSliderValue(const float Value) { BGMSoundClass->Properties.Volume = Value; BGMVolumeProgressBar->SetPercent(Value); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ApplySFXSliderValue(const float Value) { SFXSoundClass->Properties.Volume = Value; SFXVolumeProgressBar->SetPercent(Value); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void CloseSettingWidget() { SaveSettingData(GetWorld()->GetGameInstanceChecked<UKWGameInstance>()->GetSaveSettingOption()); SetRenderScale(FVector2d::Zero()); }
	
	UFUNCTION(BlueprintCallable)
	void ApplyResolutionType(FString ResolutionName, ESelectInfo::Type Info);

	UFUNCTION(BlueprintCallable)
	void ApplyFullScreen();

	UFUNCTION(BlueprintCallable)
	void ApplyWindowScreen();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class USlider> MasterSoundVolumeSlider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class USlider> BGMSoundVolumeSlider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class USlider> SFXSoundVolumeSlider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UProgressBar> MasterVolumeProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UProgressBar> BGMVolumeProgressBar;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UProgressBar> SFXVolumeProgressBar;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UComboBoxString> ResolutionComboBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UButton> UseFullScreenBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UButton> UseWindowBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UButton> CloseBtn;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundClass")
	TObjectPtr<class USoundClass> MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundClass")
	TObjectPtr<class USoundClass> BGMSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundClass")
	TObjectPtr<class USoundClass> SFXSoundClass;

	UPROPERTY()
	EResolutionTypes ResolutionTypes;

	UPROPERTY()
	uint8 bIsFullScreenActivate : 1;
	
};

