// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWLobbyWidget.h"

#include "KWFadeWidget.h"
#include "Kismet/GameplayStatics.h"
#include "XR_Project_Team10/Constant/KWLevelName.h"

void UKWLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameStartBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::GameStartBtnEvent);
	SettingBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::SettingBtnEvent);
	ExitGameBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::ExitGameBtnEvent);
	ConfirmBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::ConfirmBtnEvent);
	CancelBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::CancelBtnEvent);
	FirstEnterBtn->OnClicked.AddDynamic(this, &UKWLobbyWidget::FirstEnterBtnEvent);
	FadeWidget->FadeSequenceEndDelegate.AddUObject(this, &UKWLobbyWidget::OpenIntroLevel);
	
	LobbyUIPanel->SetRenderScale(FVector2d::Zero());
	LobbyUIPanel->SetRenderOpacity(0.f);
	LobbyUIPanel->SetIsEnabled(false);
	ExitGameCheckPanel->SetRenderScale(FVector2d::Zero());
	SettingWidget->SetRenderScale(FVector2d::Zero());
}

void UKWLobbyWidget::GameStartBtnEvent()
{
	FadeWidget->StartFadeOut();
}

void UKWLobbyWidget::SettingBtnEvent()
{
	SettingWidget->SetRenderScale(FVector2d::One());
}

void UKWLobbyWidget::ExitGameBtnEvent()
{
	ExitGameCheckPanel->SetRenderScale(FVector2d::One());
}

void UKWLobbyWidget::ConfirmBtnEvent()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UKWLobbyWidget::CancelBtnEvent()
{
	ExitGameCheckPanel->SetRenderScale(FVector2d::Zero());
}

void UKWLobbyWidget::FirstEnterBtnEvent()
{
	FirstEnterBtn->SetIsEnabled(false);
	LobbyUIPanel->SetRenderScale(FVector2d::One());
	SwapVisibilitySequence();
}

void UKWLobbyWidget::OpenIntroLevel(bool Value)
{
	// TODO: 인트로 레벨 넣기
	UGameplayStatics::OpenLevel(this, INTRO_LEVEL);
}

void UKWLobbyWidget::SwapVisibilitySequence()
{
	if(FirstEnterBtn->GetRenderOpacity() > 0.f)
	{
		FirstEnterBtn->SetRenderOpacity(FirstEnterBtn->GetRenderOpacity() - 0.01f);
	}
	if(LobbyUIPanel->GetRenderOpacity() >= 1.f)
	{
		LobbyUIPanel->SetIsEnabled(true);
		return;
	}
	LobbyUIPanel->SetRenderOpacity(LobbyUIPanel->GetRenderOpacity() + 0.005f);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWLobbyWidget::SwapVisibilitySequence);
}
