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
	HealthBar->SetRenderOpacity(1);
	ClearImage->SetRenderOpacity(0);
	ReStartBtn->SetRenderOpacity(0);
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
		ClearImage->SetRenderOpacity(1);
		ReStartBtn->SetRenderOpacity(1);
	}
	else
	{
		ClearImage->SetRenderOpacity(0);
	}
}
