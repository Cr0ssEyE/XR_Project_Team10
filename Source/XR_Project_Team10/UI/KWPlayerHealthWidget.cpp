// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWPlayerHealthWidget.h"

void UKWPlayerHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = CastChecked<AKWPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FocusedImage = VitalImageE;
	PlayerMaximumHealth = PlayerCharacter->GetHp();
	PlayerCurrentHealth = PlayerMaximumHealth;
	ImageType = EVitalImageType::Fill;
	ReStartBtn->OnClicked.AddDynamic(this, &UKWPlayerHealthWidget::ResetGame);
	
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
	// 반칸 까임
	if(PlayerCurrentHealth - PlayerCharacter->GetHp() == 1)
	{
		if(ImageType == EVitalImageType::Fill)
		{
			ImageType = EVitalImageType::Half;
			FocusedImage->SetBrushFromTexture(VitalImageSample[static_cast<int>(ImageType)]);
		}
		else if(ImageType == EVitalImageType::Half)
		{
			FocusedImage->SetBrushFromTexture(VitalImageSample[0]);
			if(FocusedImage == VitalImageB)
			{
				FocusedImage = VitalImageA;
			}
			else if(FocusedImage == VitalImageC)
			{
				FocusedImage = VitalImageB;
			}
			else if(FocusedImage == VitalImageD)
			{
				FocusedImage = VitalImageC;
			}
			else if(FocusedImage == VitalImageE)
			{
				FocusedImage = VitalImageD;
			}
			ImageType = EVitalImageType::Fill;
		}
	}
	else if(PlayerCurrentHealth - PlayerCharacter->GetHp() == 2) // 한번에 한칸 까임
	{
		if(ImageType == EVitalImageType::Fill)
		{
			FocusedImage->SetBrushFromTexture(VitalImageSample[0]);
			if(FocusedImage == VitalImageB)
			{
				FocusedImage = VitalImageA;
			}
			else if(FocusedImage == VitalImageC)
			{
				FocusedImage = VitalImageB;
			}
			else if(FocusedImage == VitalImageD)
			{
				FocusedImage = VitalImageC;
			}
			else if(FocusedImage == VitalImageE)
			{
				FocusedImage = VitalImageD;
			}
		}
		else if(ImageType == EVitalImageType::Half)
		{
			FocusedImage->SetBrushFromTexture(VitalImageSample[0]);
			if(FocusedImage == VitalImageB)
			{
				FocusedImage = VitalImageA;
			}
			else if(FocusedImage == VitalImageC)
			{
				FocusedImage = VitalImageB;
			}
			else if(FocusedImage == VitalImageD)
			{
				FocusedImage = VitalImageC;
			}
			else if(FocusedImage == VitalImageE)
			{
				FocusedImage = VitalImageD;
			}
			FocusedImage->SetBrushFromTexture(VitalImageSample[static_cast<int>(ImageType)]);
		}
	}
	PlayerCurrentHealth = PlayerCharacter->GetHp();
}
