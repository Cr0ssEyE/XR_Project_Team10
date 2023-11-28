// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"
#include "KWBossHealthWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS(BlueprintType)
class XR_PROJECT_TEAM10_API UKWBossHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetVisibleUI() { HealthBar->SetRenderOpacity(1); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetBossMonster(AKWBossMonsterHohonu* Boss) { BossMonster = Boss; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ResetGame() { UGameplayStatics::OpenLevel(GetWorld(), "MiddleTestLevel"); }
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

private:
	UPROPERTY()
	TObjectPtr<AKWBossMonsterHohonu> BossMonster;
	
	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	float CurrentHealth;
};
