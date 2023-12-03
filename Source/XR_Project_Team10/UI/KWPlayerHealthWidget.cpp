// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWPlayerHealthWidget.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"

void UKWPlayerHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// PlayerCharacter = CastChecked<AKWPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	// PlayerMaximumHealth = PlayerCharacter->GetHp();
	// PlayerCurrentHealth = PlayerMaximumHealth;
	PlayerMaximumHealth = PlayerCurrentHealth = 10;
	for (int i = 0; i < PlayerMaximumHealth; i++)
	{
		if(i % 2 != 0)
		{
			VitalImages.Emplace(CastChecked<UImage>(GetWidgetFromName(*FString::Printf(TEXT("TopHealth_%d"), i))));
		}
		else
		{
			VitalImages.Emplace(CastChecked<UImage>(GetWidgetFromName(*FString::Printf(TEXT("BottomHealth_%d"), i))));
		}
	}
	
	if(!AnimationChangeSpeed)
	{
		AnimationChangeSpeed = 0.04f;
	}
	
	FocusedImage = VitalImages[PlayerMaximumHealth - 1];
	FocusImageType = EVitalImageType::Fill;
	CurrentAnimationState = 0;
	bIsAnimationOnGoing = false;

	// TestBtn->OnClicked.AddDynamic(this, &UKWPlayerHealthWidget::AnimTestFunction);
}

void UKWPlayerHealthWidget::PlayDecreaseHealthAnimation()
{
	CurrentAnimationState++;
	if(CurrentAnimationState >= TopVitalImages.Num() || !FocusedImage)
	{
		return;
	}

	if(FocusImageType == EVitalImageType::Fill)
	{
		FocusedImage->SetBrushFromTexture(TopVitalImages[CurrentAnimationState]);
	}
	else
	{
		FocusedImage->SetBrushFromTexture(BottomVitalImages[CurrentAnimationState]);
	}
	
	if(!TopVitalImages[CurrentAnimationState] && PlayerCurrentHealth)
	{
		FocusedImage->SetRenderScale(FVector2d::Zero());
		FocusedImage = VitalImages[PlayerCurrentHealth - 1];
		CurrentAnimationState = 0;
		bIsAnimationOnGoing = false;
		
		if(FocusImageType == EVitalImageType::Fill)
		{
			FocusImageType = EVitalImageType::Half;
		}
		else
		{
			FocusImageType = EVitalImageType::Fill;
		}
		
		GetWorld()->GetTimerManager().ClearTimer(HealthAnimationTimerHandle);
	}
	if(!PlayerCurrentHealth)
	{
		FocusedImage->SetRenderScale(FVector2d::Zero());
	}
}

void UKWPlayerHealthWidget::AnimTestFunction()
{
	PlayerCurrentHealth--;
	GetWorld()->GetTimerManager().SetTimer(HealthAnimationTimerHandle, this, &UKWPlayerHealthWidget::PlayDecreaseHealthAnimation, AnimationChangeSpeed, true);
}

void UKWPlayerHealthWidget::ApplyDecreaseHealthState()
{
	if(bIsAnimationOnGoing)
	{
		return;
	}
	// TODO: 이미지 바꾸는걸 애니메이션으로 처리하기
	PlayerCurrentHealth--;
	bIsAnimationOnGoing = true;
	GetWorld()->GetTimerManager().SetTimer(HealthAnimationTimerHandle, this, &UKWPlayerHealthWidget::PlayDecreaseHealthAnimation, AnimationChangeSpeed, true);
}

void UKWPlayerHealthWidget::FillHealthState()
{
	for (int i = 0; i < VitalImages.Num(); i++)
	{
		if(i % 2 == 0)
		{
			VitalImages[i]->SetBrushFromTexture(TopVitalImages[0]);
		}
		else
		{
			VitalImages[i]->SetBrushFromTexture(BottomVitalImages[0]);
		}
		VitalImages[i]->SetRenderScale(FVector2d::One());
	}
	PlayerCurrentHealth = PlayerMaximumHealth;
}
