// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/CommonMonster/CommonMonster.h"

ACommonMonster::ACommonMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	if (nullptr != MonsterData) {
		MonsterCurrentHP = MonsterData->MonsterHP;
		MonsterCurrentMoveSpeed = MonsterData->MonsterMoveSpeed;
	}
}

void ACommonMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACommonMonster::BeginPlay()
{
	Super::BeginPlay();
}

void ACommonMonster::Research()
{
}

void ACommonMonster::Recognition()
{
}

void ACommonMonster::Tracking()
{
}

void ACommonMonster::AttackConfig()
{
}

void ACommonMonster::AttackOmen()
{
}

void ACommonMonster::Attack()
{
}

void ACommonMonster::Dead()
{
}

