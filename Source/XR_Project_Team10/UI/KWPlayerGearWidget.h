// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "XR_Project_Team10/Enumeration/KWPlayerGearType.h"
#include "KWPlayerGearWidget.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWPlayerGearWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ApplyPlayerGearState(EGearState GearState);

protected:
	virtual void NativeConstruct() override;
	void PlayGearChangeAnimation();
	
	UFUNCTION(BlueprintCallable)
	void AnimTestFunction();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (BindWidget))
	TObjectPtr<UImage> CurrentGearImage;

	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (BindWidget))
	TObjectPtr<UImage> NewGearImage;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TArray<UTexture2D*> KiwiModeImages;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TArray<UTexture2D*> GearOneImages;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TArray<UTexture2D*> GearTwoImages;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TArray<UTexture2D*> GearThreeImages;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TArray<UTexture2D*> GearFourImages;

	// UPROPERTY(EditDefaultsOnly, Category = "UI", meta=(BindWidget))
	// TObjectPtr<UButton> TestBtn;

private:
	EGearState NewGearState;

	UPROPERTY(EditAnywhere)
	float AnimationChangeSpeed;
	
	UPROPERTY()
	int CurrentAnimationState;

	UPROPERTY()
	uint8 bIsAnimationOnGoing : 1;
	
	UPROPERTY()
	FTimerHandle GearAnimationTimerHandle;
};
