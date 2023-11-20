// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KWFadeWidget.h"
#include "SpinePlugin/Public/SpineWidget.h"
#include "SpinePlugin/Public/SpinePlugin.h"
#include "Blueprint/UserWidget.h"
#include "KWGameIntroWidget.generated.h"

using namespace spine;
/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWGameIntroWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void ToggleUIEvent();

	UFUNCTION()
	void SkipCurrentSceneBtnEvent();
	
	UFUNCTION()
	void SkipIntroBtnEvent();

protected:
	void CutSceneFadeInSequence();
	void CutSceneFadeOutSequence();
	void UpdateAutoPlaySecond();
	void SwapCutSceneAndText();
	void LoadMainLevel(bool Value);
	
protected:
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	// TObjectPtr<USpineWidget> CutSceneWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UImage> ChangeTestImage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UImage> CutSceneFadeImage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> UIPanel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UTextBlock> CutSceneText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> DisableUIBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> EnableUIBtn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> SkipIntroBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> SkipCurrentCutSceneBtn;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UKWFadeWidget> FadeWidget;
	
private:
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> TestImageArray;
	
	UPROPERTY(EditAnywhere)
	TArray<USpineAtlasAsset*> SpineAtlasDataArray;

	UPROPERTY(EditAnywhere)
	TArray<USpineSkeletonDataAsset*> SpineSkeletonDataArray;
	
	UPROPERTY()
	int CurrentScene;
	
	UPROPERTY(EditAnywhere, DisplayName = "자동 재생 지연시간")
	float AutoPlaySecond;

	UPROPERTY()
	float AutoPlayElapsedSecond;
	
	UPROPERTY(EditAnywhere, DisplayName = "전체 화면 페이드 인/아웃 속도(프레임)")
	float SceneFadeSpeed;

	UPROPERTY(EditAnywhere, DisplayName = "컷 씬 페이드 인/아웃 속도(프레임)")
	float ImageFadeSpeed;
	
	UPROPERTY()
	uint8 bIsInterfaceEnabled : 1;

	UPROPERTY()
	uint8 bIsFadeSequenceOnGoing : 1;
};

