// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "KWPauseWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FResumeGameDelegate)

UENUM()
enum class ESubWidgetType : uint8
{
	None,
	ReStart,
	Exit,
};
/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FResumeGameDelegate ResumeGameDelegate;
	
protected:
	UFUNCTION()
	void ResumeBtnEvent();

	UFUNCTION()
	void ReStartBtnEvent();
	
	UFUNCTION()
	void ExitBtnEvent();

	UFUNCTION()
	void ConfirmBtnEvent();

	UFUNCTION()
	void CancelBtnEvent();
	
	void FadeSequence();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ResumeBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ReStartBtn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ExitBtn;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> BtnCheckPanel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ConfirmBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> CancelBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UImage> CheckImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TArray<UTexture2D*> CheckImageBackground;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UImage> FadeImage;

	ESubWidgetType SubWidget;
};
