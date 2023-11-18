// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KWSettingWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "KWLobbyWidget.generated.h"

class UKWFadeWidget;
/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION()
	void GameStartBtnEvent();

	UFUNCTION()
	void SettingBtnEvent();

	UFUNCTION()
	void ExitGameBtnEvent();

	UFUNCTION()
	void ConfirmBtnEvent();

	UFUNCTION()
	void CancelBtnEvent();

	UFUNCTION()
	void FirstEnterBtnEvent();
	
	UFUNCTION()
	void OpenIntroLevel(bool Value);

protected:
	void ShowLobbyUISequence();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> LobbyUIPanel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> GameStartBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> SettingBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ExitGameBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> FirstEnterBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UTextBlock> FirstEnterEmissionText;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ExitGameCheckPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ConfirmBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> CancelBtn;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UKWFadeWidget> FadeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UKWSettingWidget> SettingWidget;

private:
	UPROPERTY(EditAnywhere)
	float FadeSpeedPerTick;
	
};
