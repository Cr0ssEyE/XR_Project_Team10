// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWPlayerHealthWidget.h"

void UKWPlayerHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = CastChecked<AKWPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FocusedImage = VitalImageE;
	PlayerMaximumHealth = PlayerCharacter->GetHp();
	PlayerCurrentHealth = PlayerMaximumHealth;
}

void UKWPlayerHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(PlayerCurrentHealth != PlayerCharacter->GetHp())
	{
		ChangeImageState();
	}
}

void UKWPlayerHealthWidget::ChangeImageState()
{
	int NewHealth = PlayerCharacter->GetHp();
}
