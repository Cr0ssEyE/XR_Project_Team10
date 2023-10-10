// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "KWPlayerHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWPlayerHealthWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void ChangeImageState();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta=(BindWidget))
	TObjectPtr<UImage> VitalImageA;

	UPROPERTY(EditDefaultsOnly, Category = "UI", meta=(BindWidget))
	TObjectPtr<UImage> VitalImageB;

	UPROPERTY(EditDefaultsOnly, Category = "UI", meta=(BindWidget))
	TObjectPtr<UImage> VitalImageC;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta=(BindWidget))
	TObjectPtr<UImage> VitalImageD;

	UPROPERTY(EditDefaultsOnly, Category = "UI", meta=(BindWidget))
	TObjectPtr<UImage> VitalImageE;
	
private:
	UPROPERTY()
	TObjectPtr<AKWPlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UImage> FocusedImage;
	
	UPROPERTY()
	float PlayerMaximumHealth;
	
	UPROPERTY()
	float PlayerCurrentHealth;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UImage> FilledVitalImageSample;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UImage> HalfVitalImageSample;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UImage> EmptyVitalImageSample;
	
};
