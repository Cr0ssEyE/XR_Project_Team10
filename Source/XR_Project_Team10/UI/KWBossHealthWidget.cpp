// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/UI/KWBossHealthWidget.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

void UKWBossHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BossMonster = CastChecked<AKWBossMonsterHohonu>(UGameplayStatics::GetActorOfClass(GetWorld(), AKWBossMonsterHohonu::StaticClass()));
	MaxHealth = BossMonster->GetHp();
	CurrentHealth = MaxHealth;
	OriginImageSize = ClearImage->GetRenderTransform().Scale;
	ClearImage->SetRenderScale(FVector2D(0.f, 0.f));
	OriginBtnSize = ReStartBtn->GetRenderTransform().Scale;
	ReStartBtn->SetRenderScale(FVector2D(0.f, 0.f));
	ReStartBtn->OnClicked.AddDynamic(this, &UKWBossHealthWidget::ResetGame);
}

void UKWBossHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(!BossMonster)
	{
		return;
	}
	CurrentHealth = BossMonster->GetHp();
	float HealthBarPercent = CurrentHealth / MaxHealth;
	HealthBar->SetPercent(HealthBarPercent);
	if(CurrentHealth <= 0)
	{
		ClearImage->SetRenderScale(OriginImageSize);
		ReStartBtn->SetRenderScale(OriginBtnSize);
	}
}
