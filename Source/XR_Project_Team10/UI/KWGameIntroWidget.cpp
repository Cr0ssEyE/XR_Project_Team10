// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWGameIntroWidget.h"

#include "XR_Project_Team10/Constant/KWLevelName.h"

void UKWGameIntroWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DisableUIBtn->OnClicked.AddDynamic(this, &UKWGameIntroWidget::ToggleUIEvent);
	EnableUIBtn->OnClicked.AddDynamic(this, &UKWGameIntroWidget::ToggleUIEvent);
	SkipIntroBtn->OnClicked.AddDynamic(this, &UKWGameIntroWidget::SkipIntroBtnEvent);
	SkipCurrentCutSceneBtn->OnClicked.AddDynamic(this, &UKWGameIntroWidget::SkipCurrentSceneBtnEvent);
	
	CurrentScene = 0;
	// CutSceneWidget->Atlas = SpineAtlasDataArray[0];
	// CutSceneWidget->SkeletonData = SpineSkeletonDataArray[0];
	ChangeTestImage->SetBrushFromTexture(TestImageArray[0]);
	FadeWidget->SetFadeSpeed(SceneFadeSpeed);
	FadeWidget->FadeOutSequenceEndDelegate.AddUObject(this, &UKWGameIntroWidget::LoadMainLevel);
	
	AutoPlayElapsedSecond = 0;
	bIsFadeSequenceOnGoing = false;
	bIsInterfaceEnabled = true;
	FadeWidget->StartFadeIn();
}

void UKWGameIntroWidget::ToggleUIEvent()
{
	bIsInterfaceEnabled = !bIsInterfaceEnabled;
	UIPanel->SetRenderOpacity(bIsInterfaceEnabled);
	if(bIsInterfaceEnabled)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::UpdateAutoPlaySecond);
	}
}

void UKWGameIntroWidget::SkipCurrentSceneBtnEvent()
{
	CutSceneFadeOutSequence();
}

void UKWGameIntroWidget::SkipIntroBtnEvent()
{
	FadeWidget->StartFadeOut();
}

void UKWGameIntroWidget::CutSceneFadeInSequence()
{
	if(CutSceneFadeImage->GetRenderOpacity() >= 1)
	{
		CutSceneFadeImage->SetRenderOpacity(1.f);
		CutSceneText->SetRenderOpacity(1.f);
		
		bIsFadeSequenceOnGoing = false;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::UpdateAutoPlaySecond);
		return;
	}
	
	CutSceneFadeImage->SetRenderOpacity(CutSceneFadeImage->GetRenderOpacity() + ImageFadeSpeed);
	CutSceneText->SetRenderOpacity(CutSceneText->GetRenderOpacity() + ImageFadeSpeed);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::CutSceneFadeInSequence);
}

void UKWGameIntroWidget::CutSceneFadeOutSequence()
{
	if(CutSceneFadeImage->GetRenderOpacity() <= 0)
	{
		CutSceneFadeImage->SetRenderOpacity(0.f);
		CutSceneText->SetRenderOpacity(0.f);
		
		SwapCutSceneAndText();
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::CutSceneFadeInSequence);
		return;
	}
	CutSceneFadeImage->SetRenderOpacity(CutSceneFadeImage->GetRenderOpacity() - ImageFadeSpeed);
	CutSceneText->SetRenderOpacity(CutSceneText->GetRenderOpacity() - ImageFadeSpeed);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::CutSceneFadeOutSequence);
}

void UKWGameIntroWidget::UpdateAutoPlaySecond()
{
	if(bIsFadeSequenceOnGoing)
	{
		return;
	}
	
	if(bIsInterfaceEnabled)
	{
		AutoPlayElapsedSecond = 0;
		return;
	}

	AutoPlayElapsedSecond += GetWorld()->DeltaTimeSeconds;
	if(AutoPlayElapsedSecond == AutoPlaySecond && !bIsFadeSequenceOnGoing)
	{
		AutoPlayElapsedSecond = 0;
		bIsFadeSequenceOnGoing = true;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::CutSceneFadeOutSequence);
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::UpdateAutoPlaySecond);
}

void UKWGameIntroWidget::SwapCutSceneAndText()
{
	CurrentScene++;
	if(CurrentScene >= TestImageArray.Num())
	{
		return;
	}
	
	// CutSceneWidget->Atlas = SpineAtlasDataArray[CurrentScene];
	// CutSceneWidget->SkeletonData = SpineSkeletonDataArray[CurrentScene];
	ChangeTestImage->SetBrushFromTexture(TestImageArray[CurrentScene]);
	// TODO:: CSV에서 읽어오기
	CutSceneText->SetText(FText::FromString(FString(TEXT(""))));
}

void UKWGameIntroWidget::LoadMainLevel(bool Value)
{
	UGameplayStatics::OpenLevel(GetWorld(), PLAY_LEVEL);
}
