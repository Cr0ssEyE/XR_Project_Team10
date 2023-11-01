// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWFadeWidget.h"

#include "Kismet/GameplayStatics.h"

void UKWFadeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FadeImage->SetRenderOpacity(0.f);
	FadeImage->SetRenderScale(FVector2D::ZeroVector);
	bIsFading = false;
}

void UKWFadeWidget::StartFade()
{
	if(bIsFading)
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWFadeWidget::FadeSequence);
	bIsFading = true;
}

void UKWFadeWidget::FadeSequence()
{
	FadeImage->SetRenderOpacity(FadeImage->GetRenderOpacity() + 0.005f);
	if(FadeImage->GetRenderOpacity() >= 1.f)
	{
		UGameplayStatics::OpenLevel(this, GetWorld()->OriginalWorldName);
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWFadeWidget::FadeSequence);
}
