// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "KWPauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void ResumeBtnEvent();

	UFUNCTION()
	void ReStartBtnEvent();
	
	UFUNCTION()
	void ExitBtnEvent();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ResumeBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ReStartBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ExitBtn;
	
};
