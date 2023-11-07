// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "KWFadeWidget.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWFadeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void StartFade();
	
protected:
	void FadeSequence();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UImage> FadeImage;

	uint8 bIsFading : 1;
};
