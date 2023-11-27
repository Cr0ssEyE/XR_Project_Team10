// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWLobbyWidget.h"

#include "KWFadeWidget.h"
#include "Kismet/GameplayStatics.h"
#include "XR_Project_Team10/Constant/KWLevelName.h"
#include "XR_Project_Team10/Game/KWGameInstance.h"

void UKWLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameStartBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::GameStartBtnEvent);
	SettingBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::SettingBtnEvent);
	ExitGameBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::ExitGameBtnEvent);
	ConfirmBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::ConfirmBtnEvent);
	CancelBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::CancelBtnEvent);
	FirstEnterBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::FirstEnterBtnEvent);
	FadeWidget->FadeOutSequenceEndDelegate.AddUObject(this, &UKWLobbyWidget::OpenIntroLevel);
	
	LobbyUIPanel->SetRenderScale(FVector2d::Zero());
	LobbyUIPanel->SetRenderOpacity(0.f);
	LobbyUIPanel->SetIsEnabled(false);
	ExitGameCheckPanel->SetRenderScale(FVector2d::Zero());
	SettingWidget->SetRenderScale(FVector2d::Zero());
	FadeWidget->SetFadeSpeed(FadeSpeedPerTick);
	FadeWidget->StartFadeIn();
}

void UKWLobbyWidget::GameStartBtnEvent()
{
	TObjectPtr<UKWGameInstance> CurrentGI = CastChecked<UKWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	TObjectPtr<UKWSaveSettingOption> SaveSettingOption = CurrentGI->GetSaveSettingOption();
	if(SaveSettingOption)
	{
		SettingWidget->SaveSettingData(SaveSettingOption);
	}
	LobbyUIPanel->SetIsEnabled(false);
	FadeWidget->StartFadeOut();
}

void UKWLobbyWidget::SettingBtnEvent()
{
	bool IsOpened = !SettingWidget->GetRenderTransform().Scale.X;
	SettingWidget->SetRenderScale(FVector2d::One() * IsOpened);
}

void UKWLobbyWidget::ExitGameBtnEvent()
{
	ExitGameCheckPanel->SetRenderScale(FVector2d::One());
}

void UKWLobbyWidget::ConfirmBtnEvent()
{
	TObjectPtr<UKWGameInstance> CurrentGI = CastChecked<UKWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	TObjectPtr<UKWSaveSettingOption> SaveSettingOption = CurrentGI->GetSaveSettingOption();
	if(SaveSettingOption)
	{
		SettingWidget->SaveSettingData(SaveSettingOption);
	}
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UKWLobbyWidget::CancelBtnEvent()
{
	ExitGameCheckPanel->SetRenderScale(FVector2d::Zero());
}

void UKWLobbyWidget::FirstEnterBtnEvent()
{
	FirstEnterBtn->SetIsEnabled(false);
	FirstEnterBtn->SetRenderScale(FVector2d::Zero());
	LobbyUIPanel->SetRenderScale(FVector2d::One());
	
	ShowLobbyUISequence();
}

void UKWLobbyWidget::OpenIntroLevel(bool Value)
{
	// TODO: 인트로 레벨 넣기
	UGameplayStatics::OpenLevel(this, INTRO_LEVEL);
}

void UKWLobbyWidget::ShowLobbyUISequence()
{
	if(FirstEnterEmissionText->GetRenderOpacity() > 0.f)
	{
		FirstEnterEmissionText->SetRenderOpacity(FirstEnterBtn->GetRenderOpacity() - FadeSpeedPerTick * 2);
		if(FirstEnterEmissionText->GetRenderOpacity() <= 0.f)
		{
			FirstEnterEmissionText->SetRenderScale(FVector2d::Zero());
			FirstEnterEmissionText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if(LobbyUIPanel->GetRenderOpacity() >= 1.f)
	{
		LobbyUIPanel->SetIsEnabled(true);
		return;
	}
	LobbyUIPanel->SetRenderOpacity(LobbyUIPanel->GetRenderOpacity() + FadeSpeedPerTick);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWLobbyWidget::ShowLobbyUISequence);
}
