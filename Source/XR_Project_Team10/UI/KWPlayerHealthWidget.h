// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "KWPlayerHealthWidget.generated.h"

class AKWPlayerCharacter;

UENUM()
enum class EVitalImageType
{
	Half,
	Fill
};

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWPlayerHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ApplyDecreaseHealthState();
	void FillHealthState();
	
protected:
	virtual void NativeConstruct() override;
	void PlayDecreaseHealthAnimation();
	
	UFUNCTION(BlueprintCallable)
	void AnimTestFunction();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TArray<UImage*> VitalImages;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TArray<UTexture2D*> TopVitalImages;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TArray<UTexture2D*> BottomVitalImages;

	// UPROPERTY(EditDefaultsOnly, Category = "UI", meta=(BindWidget))
	// TObjectPtr<UButton> TestBtn;
	
private:
	UPROPERTY()
	TObjectPtr<AKWPlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UImage> FocusedImage;

	UPROPERTY()
	EVitalImageType FocusImageType;

	UPROPERTY(EditAnywhere)
	float AnimationChangeSpeed;
	
	UPROPERTY()
	float PlayerMaximumHealth;
	
	UPROPERTY()
	float PlayerCurrentHealth;
	
	UPROPERTY()
	int CurrentAnimationState;

	UPROPERTY()
	uint8 bIsAnimationOnGoing : 1;

	UPROPERTY()
	FTimerHandle HealthAnimationTimerHandle;
	
};
