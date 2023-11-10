// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWFadeWidget.h"

#include "Kismet/GameplayStatics.h"

void UKWFadeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// FadeImage->SetRenderOpacity(0.f);
	// FadeImage->SetRenderScale(FVector2D::ZeroVector);
	bIsFading = false;
}

void UKWFadeWidget::StartFadeOut()
{
	if(bIsFading)
	{
		return;
	}
	FadeImage->SetRenderScale(FVector2D::One());
	FadeImage->SetRenderOpacity(0.f);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWFadeWidget::FadeOutSequence);
	bIsFading = true;
}

void UKWFadeWidget::StartFadeIn()
{
	if(bIsFading)
	{
		return;
	}
	FadeImage->SetRenderScale(FVector2D::One());
	FadeImage->SetRenderOpacity(1.0f);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWFadeWidget::FadeInSequence);
	bIsFading = true;
}

void UKWFadeWidget::FadeOutSequence()
{
	FadeImage->SetRenderOpacity(FadeImage->GetRenderOpacity() + 0.0025f);
	if(FadeImage->GetRenderOpacity() >= 1.f)
	{
		UGameplayStatics::OpenLevel(this, GetWorld()->OriginalWorldName);
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWFadeWidget::FadeOutSequence);
}

void UKWFadeWidget::FadeInSequence()
{
	FadeImage->SetRenderOpacity(FadeImage->GetRenderOpacity() - 0.0025f);
	if(FadeImage->GetRenderOpacity() <= 0.f)
	{
		bIsFading = false;
		FadeImage->SetRenderScale(FVector2D::Zero());
		FadeSequenceEndDelegate.Broadcast(true);
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UKWFadeWidget::FadeInSequence);
}
