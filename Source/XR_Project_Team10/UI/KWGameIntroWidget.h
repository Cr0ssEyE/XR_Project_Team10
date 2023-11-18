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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<USpineWidget> CutSceneWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UTextBlock> CutSceneText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> DisableUIBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> EnableUIBtn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> SkipBtn;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UKWFadeWidget> FadeWidget;
	
private:
	UPROPERTY(EditAnywhere)
	TMap<USpineAtlasAsset*, USpineSkeletonDataAsset*> CutSceneData;
	
};
