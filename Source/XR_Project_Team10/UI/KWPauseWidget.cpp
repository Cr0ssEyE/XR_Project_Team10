// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWPauseWidget.h"

#include "Kismet/GameplayStatics.h"
#include "XR_Project_Team10/Player/KWPlayerController.h"

void UKWPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ResumeBtn->OnClicked.AddDynamic(this, &UKWPauseWidget::ResumeBtnEvent);
	ReStartBtn->OnClicked.AddDynamic(this, &UKWPauseWidget::ReStartBtnEvent);
	ExitBtn->OnClicked.AddDynamic(this, &UKWPauseWidget::ExitBtnEvent);
	ConfirmBtn->OnClicked.AddDynamic(this, &UKWPauseWidget::ConfirmBtnEvent);
	CancelBtn->OnClicked.AddDynamic(this, &UKWPauseWidget::CancelBtnEvent);
	
	FadeImage->SetRenderOpacity(0.f);
	FadeImage->SetRenderScale(FVector2D::ZeroVector);
	BtnCheckPanel->SetRenderScale(FVector2D::ZeroVector);
	SubWidget = ESubWidgetType::None;
}

void UKWPauseWidget::ResumeBtnEvent()
{
	ResumeGameDelegate.Broadcast();
}

void UKWPauseWidget::ReStartBtnEvent()
{
	SubWidget = ESubWidgetType::ReStart;
	CheckImage->SetBrushFromTexture(CheckImageBackground[static_cast<uint8>(SubWidget) - 1]);
	BtnCheckPanel->SetRenderScale(FVector2D::One());
}

void UKWPauseWidget::ExitBtnEvent()
{
	SubWidget = ESubWidgetType::Exit;
	CheckImage->SetBrushFromTexture(CheckImageBackground[static_cast<uint8>(SubWidget) - 1]);
	BtnCheckPanel->SetRenderScale(FVector2D::One());
}

void UKWPauseWidget::ConfirmBtnEvent()
{
	AKWPlayerController* PlayerController = Cast<AKWPlayerController>(GetWorld()->GetFirstPlayerController());
	if(PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
	}
	FadeImage->SetRenderScale(FVector2D::One());
	FadeSequence();
}

void UKWPauseWidget::CancelBtnEvent()
{
	SubWidget = ESubWidgetType::None;
	BtnCheckPanel->SetRenderScale(FVector2D::ZeroVector);
}

void UKWPauseWidget::FadeSequence()
{
	FadeImage->SetRenderOpacity(FadeImage->GetRenderOpacity() + 0.005f);
	if(FadeImage->GetRenderOpacity() >= 1.f)
	{
		switch (SubWidget)
		{
		case ESubWidgetType::ReStart:
			UGameplayStatics::OpenLevel(this, GetWorld()->OriginalWorldName);
			break;
			
			// TODO: 로비 레벨 만들어서 연결하기
		case ESubWidgetType::Exit:
			UGameplayStatics::OpenLevel(this, GetWorld()->OriginalWorldName);
			break;
		default:
			checkNoEntry();
		}
		return;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWPauseWidget::FadeSequence);
}
