// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Player/KWPlayerWidgetController.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

// Sets default values for this component's properties
UKWPlayerWidgetController::UKWPlayerWidgetController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PauseWidget = CreateDefaultSubobject<UKWPauseWidget>(TEXT("PauseWidget"));
	PauseWidgetClass = FPPConstructorHelper::FindAndGetClass<UKWPauseWidget>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Rolling-Kiwi/Blueprint/UI/WB_PauseWidget.WB_PauseWidget_C'"), EAssertionLevel::Check);
	
	ScreenFadeWidget = CreateDefaultSubobject<UKWFadeWidget>(TEXT("FadeWidget"));
	ScreenFadeWidgetClass = FPPConstructorHelper::FindAndGetClass<UKWFadeWidget>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Rolling-Kiwi/Blueprint/UI/WB_FadeWidget.WB_FadeWidget_C'"), EAssertionLevel::Check);
	
	PlayerHealthWidget = CreateDefaultSubobject<UKWPlayerHealthWidget>(TEXT("PlayerHealthWidget"));
	PlayerHealthWidgetClass = FPPConstructorHelper::FindAndGetClass<UKWPlayerHealthWidget>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Rolling-Kiwi/Blueprint/UI/WB_PlayerHealthWidget.WB_PlayerHealthWidget_C'"), EAssertionLevel::Check);
	
	PlayerGearWidget = CreateDefaultSubobject<UKWPlayerGearWidget>(TEXT("PlayerGearWidget"));
	PlayerGearWidgetClass = FPPConstructorHelper::FindAndGetClass<UKWPlayerGearWidget>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Rolling-Kiwi/Blueprint/UI/WB_PlayerGearWidget.WB_PlayerGearWidget_C'"), EAssertionLevel::Check);

	bIsVisible = true;
}


// Called when the game starts
void UKWPlayerWidgetController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(IsValid(PauseWidgetClass))
	{
		PauseWidget = CastChecked<UKWPauseWidget>(CreateWidget(GetWorld(), PauseWidgetClass));
		if(PauseWidget)
		{
			PauseWidget->SetVisibility(ESlateVisibility::Hidden);
			PauseWidget->SetIsEnabled(false);
		}
	}
	
	if(IsValid(ScreenFadeWidgetClass))
	{
		ScreenFadeWidget = CastChecked<UKWFadeWidget>(CreateWidget(GetWorld(), ScreenFadeWidgetClass));
		if(ScreenFadeWidget)
		{
			ScreenFadeWidget->SetVisibility(ESlateVisibility::Hidden);
			ScreenFadeWidget->SetIsEnabled(false);
			ScreenFadeWidget->FadeOutSequenceEndDelegate.AddUObject(this, &UKWPlayerWidgetController::LoadCurrentLevel);
		}
	}
	
	if(IsValid(PlayerHealthWidgetClass))
	{
		PlayerHealthWidget = CastChecked<UKWPlayerHealthWidget>(CreateWidget(GetWorld(), PlayerHealthWidgetClass));
	}
	
	if(IsValid(PlayerGearWidgetClass))
	{
		PlayerGearWidget = CastChecked<UKWPlayerGearWidget>(CreateWidget(GetWorld(), PlayerGearWidgetClass));
	}

	PauseWidget->ResumeGameDelegate.AddUObject(this, &UKWPlayerWidgetController::TogglePauseWidget);
}


// Called every frame
void UKWPlayerWidgetController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKWPlayerWidgetController::TogglePauseWidget()
{
	if(PauseWidget->IsVisible())
	{
		FInputModeGameOnly GameOnlyInputMode;
		GameOnlyInputMode.SetConsumeCaptureMouseDown(false);
		GetWorld()->GetFirstPlayerController()->SetInputMode(GameOnlyInputMode);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
		PauseWidget->SetVisibility(ESlateVisibility::Hidden);
		PauseWidget->SetIsEnabled(false);
	}
	else
	{
		FInputModeUIOnly UIOnlyInputMode;
		GetWorld()->GetFirstPlayerController()->SetInputMode(UIOnlyInputMode);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.00001f);
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
		PauseWidget->SetIsEnabled(true);
	}
}

void UKWPlayerWidgetController::UpdateHealthWidget(bool FillHealth)
{
	if(FillHealth)
	{
		PlayerHealthWidget->FillHealthState();
	}
	else
	{
		PlayerHealthWidget->ApplyDecreaseHealthState();
	}
}

void UKWPlayerWidgetController::UpdateGearWidget(EGearState GearState)
{
	PlayerGearWidget->ApplyPlayerGearState(GearState);
}

void UKWPlayerWidgetController::StartFadeOut()
{
	ScreenFadeWidget->SetVisibility(ESlateVisibility::Visible);
	ScreenFadeWidget->SetIsEnabled(true);
	ScreenFadeWidget->StartFadeOut();
}

void UKWPlayerWidgetController::ToggleAllWidgetVisibility()
{
	if(bIsVisible)
	{
		PlayerHealthWidget->RemoveFromParent();
		PlayerGearWidget->RemoveFromParent();
		ScreenFadeWidget->RemoveFromParent();
		PauseWidget->RemoveFromParent();
	}
	else
	{
		PlayerHealthWidget->AddToViewport();
		PlayerGearWidget->AddToViewport();
		ScreenFadeWidget->AddToViewport();
		PauseWidget->AddToViewport();
	}
	bIsVisible = !bIsVisible;
}

