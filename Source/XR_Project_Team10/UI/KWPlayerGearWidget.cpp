// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWPlayerGearWidget.h"

void UKWPlayerGearWidget::NativeConstruct()
{
	Super::NativeConstruct();
	NewGearState = EGearState::KiwiMode;

	if(!AnimationChangeSpeed)
	{
		AnimationChangeSpeed = 0.04f;
	}
	CurrentAnimationState = 0;
	bIsAnimationOnGoing = false;
	NewGearImage->SetRenderScale(FVector2d::Zero());
	// TestBtn->OnClicked.AddDynamic(this, &UKWPlayerGearWidget::AnimTestFunction);
}


void UKWPlayerGearWidget::ApplyPlayerGearState(EGearState GearState)
{
	if(bIsAnimationOnGoing || NewGearState == GearState)
	{	
		return;
	}
	
	NewGearState = GearState;
	CurrentGearImage->SetRenderScale(FVector2d::Zero());
	GetWorld()->GetTimerManager().SetTimer(GearAnimationTimerHandle, this, &UKWPlayerGearWidget::PlayGearChangeAnimation, AnimationChangeSpeed, true);
}


void UKWPlayerGearWidget::PlayGearChangeAnimation()
{
	UTexture2D* NextTexture2D = nullptr;
	switch (NewGearState)
	{
	case EGearState::GearOne:
		if(CurrentAnimationState < GearOneImages.Num())
		{
			NextTexture2D = GearOneImages[CurrentAnimationState];
		}
		break;
	case EGearState::GearTwo:
		if(CurrentAnimationState < GearTwoImages.Num())
		{
			NextTexture2D = GearTwoImages[CurrentAnimationState];
		}
		break;
	case EGearState::GearThree:
		if(CurrentAnimationState < GearThreeImages.Num())
		{
			NextTexture2D = GearThreeImages[CurrentAnimationState];
		}
		break;
	case EGearState::GearFour:
		if(CurrentAnimationState < GearFourImages.Num())
		{
			NextTexture2D = GearFourImages[CurrentAnimationState];
		}
		break;
	case EGearState::KiwiMode:
		if(CurrentAnimationState < KiwiModeImages.Num())
		{
			NextTexture2D = KiwiModeImages[CurrentAnimationState];
		}
		break;
	default:
		checkNoEntry();
	}
	if(!NextTexture2D)
	{
		CurrentGearImage->SetRenderScale(FVector2d::One());
		NewGearImage->SetBrushFromTexture(nullptr);
		NewGearImage->SetRenderScale(FVector2d::Zero());
		bIsAnimationOnGoing = false;
		CurrentAnimationState = 0;
		
		GetWorld()->GetTimerManager().ClearTimer(GearAnimationTimerHandle);
		return;
	}
	NewGearImage->SetRenderScale(FVector2d::One());
	NewGearImage->SetBrushFromTexture(NextTexture2D);
	CurrentGearImage->SetBrushFromTexture(NextTexture2D);
	CurrentAnimationState++;
}

void UKWPlayerGearWidget::AnimTestFunction()
{
	int NextGear = 0;
	if(NewGearState != EGearState::KiwiMode)
	{
		NextGear = static_cast<int>(NewGearState) + 1;
	}
	CurrentGearImage->SetRenderScale(FVector2d::Zero());
	NewGearState = static_cast<EGearState>(NextGear);
	GetWorld()->GetTimerManager().SetTimer(GearAnimationTimerHandle, this, &UKWPlayerGearWidget::PlayGearChangeAnimation, AnimationChangeSpeed, true);
}
