// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWPlayerGearWidget.h"

void UKWPlayerGearWidget::NativeConstruct()
{
	Super::NativeConstruct();
	NewGearState = EGearState::EndOfGearState;

	if(!AnimationChangeSpeed)
	{
		AnimationChangeSpeed = 0.04f;
	}
	CurrentAnimationState = 0;
	bIsAnimationOnGoing = false;
	NewGearImage->SetRenderScale(FVector2d::Zero());
	TestBtn->OnClicked.AddDynamic(this, &UKWPlayerGearWidget::AnimTestFunction);
}


void UKWPlayerGearWidget::ApplyPlayerGearState(EGearState GearState)
{
	if(bIsAnimationOnGoing)
	{	
		return;
	}
	NewGearState = GearState;
	NewGearImage->SetRenderScale(FVector2d::One());
	GetWorld()->GetTimerManager().SetTimer(GearAnimationTimerHandle, this, &UKWPlayerGearWidget::PlayGearChangeAnimation, AnimationChangeSpeed, true);
}


void UKWPlayerGearWidget::PlayGearChangeAnimation()
{
	CurrentAnimationState++;
	UTexture2D* NextTexture2D = nullptr;
	switch (NewGearState)
	{
	case EGearState::GearOne:
		NextTexture2D = GearOneImages[CurrentAnimationState];
		break;
	case EGearState::GearTwo:
		NextTexture2D = GearTwoImages[CurrentAnimationState];
		break;
	case EGearState::GearThree:
		NextTexture2D = GearThreeImages[CurrentAnimationState];
		break;
	case EGearState::GearFour:
		NextTexture2D = GearFourImages[CurrentAnimationState];
		break;
	default:
		checkNoEntry();
	}
	if(!GearOneImages[CurrentAnimationState + 1])
	{
		CurrentGearImage->SetBrushFromTexture(NextTexture2D);
		NewGearImage->SetBrushFromTexture(nullptr);
		NewGearImage->SetRenderScale(FVector2d::Zero());
		bIsAnimationOnGoing = false;
		CurrentAnimationState = 0;
		
		GetWorld()->GetTimerManager().ClearTimer(GearAnimationTimerHandle);
		return;
	}
	NewGearImage->SetBrushFromTexture(NextTexture2D);
}

void UKWPlayerGearWidget::AnimTestFunction()
{
	int NextGear = 0;
	if(NewGearState != EGearState::EndOfGearState)
	{
		NextGear = static_cast<int>(NewGearState) + 1;
		NewGearState = static_cast<EGearState>(NextGear);
		NewGearImage->SetRenderScale(FVector2d::One());
	}
	NewGearState = static_cast<EGearState>(NextGear);
	NewGearImage->SetRenderScale(FVector2d::One());
	GetWorld()->GetTimerManager().SetTimer(GearAnimationTimerHandle, this, &UKWPlayerGearWidget::PlayGearChangeAnimation, AnimationChangeSpeed, true);
}