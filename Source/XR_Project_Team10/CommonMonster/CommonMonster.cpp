// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/CommonMonster/CommonMonster.h"

ACommonMonster::ACommonMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	MonsterCurrentHP = MonsterData->MonsterHP;
	MonsterCurrentMoveSpeed = MonsterData->MonsterMoveSpeed;
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

uint32 ACommonMonster::GetMonsterHP()
{
	return MonsterCurrentHP;
}

void ACommonMonster::SetMonsterHP(uint32 hp)
{
	MonsterCurrentHP = hp;
}

EState ACommonMonster::GetMonsterState()
{
	return MonsterState;
}

