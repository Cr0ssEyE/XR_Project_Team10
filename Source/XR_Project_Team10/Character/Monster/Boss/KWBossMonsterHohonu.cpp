// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"

AKWBossMonsterHohonu::AKWBossMonsterHohonu()
{
	
}

float AKWBossMonsterHohonu::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AKWBossMonsterHohonu::PlayEncounterAnimation()
{
	Super::PlayEncounterAnimation();
	
}

void AKWBossMonsterHohonu::PlayDeadAnimation()
{
	Super::PlayDeadAnimation();
	
}

void AKWBossMonsterHohonu::DeActivateInGame()
{
	Super::DeActivateInGame();
	
}

void AKWBossMonsterHohonu::ReActivateInGame()
{
	Super::ReActivateInGame();
	
}

void AKWBossMonsterHohonu::ExecutePattern_SC(UBehaviorTreeComponent& OwnerComp)
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_SL(UBehaviorTreeComponent& OwnerComp)
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_MA(UBehaviorTreeComponent& OwnerComp)
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_WW(UBehaviorTreeComponent& OwnerComp)
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_BS(UBehaviorTreeComponent& OwnerComp)
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_ML(UBehaviorTreeComponent& OwnerComp)
{
	
}

void AKWBossMonsterHohonu::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKWBossMonsterHohonu::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AKWBossMonsterHohonu::InitData()
{
	Super::InitData();
	
}
