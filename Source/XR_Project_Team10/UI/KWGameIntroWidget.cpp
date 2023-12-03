// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWGameIntroWidget.h"

#include "XR_Project_Team10/Constant/KWLevelName.h"
#include "XR_Project_Team10/Util/StringDataTable.h"

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
	const FStringDataTable* PrologueString = CutSceneTextDataArray[0].GetRow<FStringDataTable>(CutSceneTextDataArray[0].RowName.ToString());
	CutSceneText->SetText(FText::FromName(PrologueString->Kor));
	ChangeTestImage->SetBrushFromTexture(TestImageArray[0]);

	EnableUIBtn->SetRenderScale(FVector2d::Zero());
	ActivateSkipAlarm->SetRenderScale(FVector2d::Zero());
	FadeWidget->SetFadeSpeed(SceneFadeSpeed);
	FadeWidget->FadeOutSequenceEndDelegate.AddUObject(this, &UKWGameIntroWidget::LoadMainLevel);
	AutoPlayElapsedSecond = 0;
	bIsFadeSequenceOnGoing = false;
	bIsInterfaceEnabled = true;
	bIsFirstText = true;
	FadeWidget->StartFadeIn();
	CutSceneFadeInSequence();
}

void UKWGameIntroWidget::ToggleUIEvent()
{
	bIsInterfaceEnabled = !bIsInterfaceEnabled;
	UIPanel->SetRenderOpacity(bIsInterfaceEnabled);
	EnableUIBtn->SetRenderScale(FVector2d::One() * !bIsInterfaceEnabled);
	SkipCurrentCutSceneBtn->SetRenderScale(FVector2d::One() * bIsInterfaceEnabled);
	if(bIsInterfaceEnabled)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::UpdateAutoPlaySecond);
	}
}

void UKWGameIntroWidget::SkipCurrentSceneBtnEvent()
{
	SkipCurrentCutSceneBtn->SetIsEnabled(false);
	if(CurrentScene <= TestImageArray.Num())
	{
		AutoPlayElapsedSecond = 0;
		bIsFadeSequenceOnGoing = true;
		ActivateSkipAlarm->SetRenderScale(FVector2d::Zero());
		if(bIsFirstText)
		{
			TextFadeOutSequence();
		}
		else
		{
			CutSceneFadeOutSequence();
		}
	}
}

void UKWGameIntroWidget::SkipIntroBtnEvent()
{
	SkipIntroBtn->SetIsEnabled(false);
	SkipCurrentCutSceneBtn->SetIsEnabled(false);
	AutoPlayElapsedSecond = 0;
	bIsFadeSequenceOnGoing = true;
	FadeWidget->StartFadeOut();
}

void UKWGameIntroWidget::CutSceneFadeInSequence()
{
	if(CutSceneFadeImage->GetRenderOpacity() <= 0)
	{
		CutSceneFadeImage->SetRenderOpacity(0.f);
		CutSceneText->SetRenderOpacity(1.f);
		ActivateSkipAlarm->SetRenderScale(FVector2d::One());
		
		bIsFadeSequenceOnGoing = false;
		SkipCurrentCutSceneBtn->SetIsEnabled(true);
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::UpdateAutoPlaySecond);
		return;
	}
	
	CutSceneFadeImage->SetRenderOpacity(CutSceneFadeImage->GetRenderOpacity() - ImageFadeSpeed);
	CutSceneText->SetRenderOpacity(CutSceneText->GetRenderOpacity() + ImageFadeSpeed);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::CutSceneFadeInSequence);
}

void UKWGameIntroWidget::CutSceneFadeOutSequence()
{
	if(CutSceneFadeImage->GetRenderOpacity() >= 1)
	{
		CutSceneFadeImage->SetRenderOpacity(1.f);
		CutSceneText->SetRenderOpacity(0.f);
		
		SwapCutSceneAndText();
		if(CurrentScene <= TestImageArray.Num())
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::CutSceneFadeInSequence);
		}
		return;
	}
	CutSceneFadeImage->SetRenderOpacity(CutSceneFadeImage->GetRenderOpacity() + ImageFadeSpeed);
	CutSceneText->SetRenderOpacity(CutSceneText->GetRenderOpacity() - ImageFadeSpeed);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::CutSceneFadeOutSequence);
}

void UKWGameIntroWidget::TextFadeInSequence()
{
	if(CutSceneText->GetRenderOpacity() >= 1)
	{
		CutSceneText->SetRenderOpacity(1.f);
		ActivateSkipAlarm->SetRenderScale(FVector2d::One());
		
		bIsFadeSequenceOnGoing = false;
		SkipCurrentCutSceneBtn->SetIsEnabled(true);
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::UpdateAutoPlaySecond);
		return;
	}
	
	CutSceneText->SetRenderOpacity(CutSceneText->GetRenderOpacity() + ImageFadeSpeed);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::TextFadeInSequence);
}

void UKWGameIntroWidget::TextFadeOutSequence()
{
	if(CutSceneText->GetRenderOpacity() <= 0)
	{
		CutSceneText->SetRenderOpacity(0.f);
		
		SwapCutSceneAndText();
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::TextFadeInSequence);
		return;
	}

	CutSceneText->SetRenderOpacity(CutSceneText->GetRenderOpacity() - ImageFadeSpeed);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWGameIntroWidget::TextFadeOutSequence);
}

void UKWGameIntroWidget::UpdateAutoPlaySecond()
{
	if(bIsFadeSequenceOnGoing)
	{
		AutoPlayElapsedSecond = 0;
		return;
	}
	
	if(!bIsInterfaceEnabled)
	{
		AutoPlayElapsedSecond = 0;
		return;
	}

	AutoPlayElapsedSecond += GetWorld()->DeltaTimeSeconds;
	if(AutoPlayElapsedSecond >= AutoPlaySecond && !bIsFadeSequenceOnGoing)
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
	if(bIsFirstText)
	{
		const FStringDataTable* PrologueString = CutSceneTextDataArray[CurrentScene].GetRow<FStringDataTable>(CutSceneTextDataArray[CurrentScene].RowName.ToString());
		CutSceneText->SetText(FText::FromName(PrologueString->Kor));
		bIsFirstText = false;
		return;
	}
	
	if(CurrentScene >= TestImageArray.Num())
	{
		bIsFadeSequenceOnGoing = true;
		SkipCurrentCutSceneBtn->SetIsEnabled(false);
		SkipIntroBtn->SetIsEnabled(false);
		DisableUIBtn->SetIsEnabled(false);
		FadeWidget->StartFadeOut();
		return;
	}

	const FStringDataTable* PrologueString = CutSceneTextDataArray[CurrentScene].GetRow<FStringDataTable>(CutSceneTextDataArray[CurrentScene].RowName.ToString());
	CutSceneText->SetText(FText::FromName(PrologueString->Kor));
	ChangeTestImage->SetBrushFromTexture(TestImageArray[CurrentScene]);
	bIsFirstText = true;
}

void UKWGameIntroWidget::LoadMainLevel(bool Value)
{
	UGameplayStatics::OpenLevel(GetWorld(), PLAY_LEVEL);
}
