// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "KWFadeWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FFadeInSequenceEndDelegate, bool)
DECLARE_MULTICAST_DELEGATE_OneParam(FFadeOutSequenceEndDelegate, bool)

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWFadeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void StartFadeIn();
	void StartFadeOut();

	FORCEINLINE float GetFadeOpacity() const { return FadeImage->GetRenderOpacity(); }
	FORCEINLINE void SetFadeSpeed(float Value) { FadeSpeedPerTick = Value; }
	
	FFadeInSequenceEndDelegate FadeInSequenceEndDelegate;
	FFadeOutSequenceEndDelegate FadeOutSequenceEndDelegate;
	
protected:
	void FadeInSequence();
	void FadeOutSequence();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UImage> FadeImage;

private:
	UPROPERTY(EditAnywhere)
	float FadeSpeedPerTick;
	
	uint8 bIsFading : 1;
};
