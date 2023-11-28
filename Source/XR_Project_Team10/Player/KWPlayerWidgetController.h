// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "XR_Project_Team10/Constant/KWLevelName.h"
#include "XR_Project_Team10/UI/KWFadeWidget.h"
#include "XR_Project_Team10/UI/KWPauseWidget.h"
#include "XR_Project_Team10/UI/KWPlayerGearWidget.h"
#include "XR_Project_Team10/UI/KWPlayerHealthWidget.h"
#include "XR_Project_Team10/Enumeration/KWPlayerGearType.h"
#include "KWPlayerWidgetController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class XR_PROJECT_TEAM10_API UKWPlayerWidgetController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKWPlayerWidgetController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TogglePauseWidget();
	void UpdateHealthWidget(bool FillHealth);
	void UpdateGearWidget(EGearState GearState);
	void StartFadeOut();
	
	UFUNCTION(BlueprintCallable)
	void ToggleAllWidgetVisibility();
	
private:
	FORCEINLINE void LoadCurrentLevel(bool Value) { UGameplayStatics::OpenLevel(GetWorld(), PLAY_LEVEL); }
	
private:
	UPROPERTY()
	TSubclassOf<UKWPauseWidget> PauseWidgetClass;

	UPROPERTY()
	TObjectPtr<UKWPauseWidget> PauseWidget;

	UPROPERTY()
	TSubclassOf<UKWFadeWidget> ScreenFadeWidgetClass;

	UPROPERTY()
	TObjectPtr<UKWFadeWidget> ScreenFadeWidget;

	UPROPERTY()
	TSubclassOf<UKWPlayerHealthWidget> PlayerHealthWidgetClass;

	UPROPERTY()
	TObjectPtr<UKWPlayerHealthWidget> PlayerHealthWidget;

	UPROPERTY()
	TSubclassOf<UKWPlayerGearWidget> PlayerGearWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UKWPlayerGearWidget> PlayerGearWidget;

	uint8 bIsVisible : 1;
};
