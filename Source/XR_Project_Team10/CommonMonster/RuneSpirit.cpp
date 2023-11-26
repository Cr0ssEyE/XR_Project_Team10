// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/CommonMonster/RuneSpirit.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

ARuneSpirit::ARuneSpirit()
{
	DeadMontage = FPPConstructorHelper::FindAndGetObject<UAnimMontage>(TEXT("/Script/Engine.AnimMontage'/Game/1-Graphic-Resource/Monster/RuneSpirit/Animation/AM_RuneSpirit_Dead.AM_RuneSpirit_Dead'"));
	MonsterData = FPPConstructorHelper::FindAndGetObject<UCommonMonsterDataAsset>(TEXT("/Game/Rolling-Kiwi/Datas/DataAssets/RuneSpiritDataAsset"));
}

void ARuneSpirit::BeginPlay()
{
	Super::BeginPlay();
}

void ARuneSpirit::AttackOmen()
{
	Super::AttackOmen();
}

void ARuneSpirit::Attack()
{
	Super::Attack();
}

void ARuneSpirit::AttackEnd()
{
	Super::AttackEnd();
}

void ARuneSpirit::AttackBehaviour()
{

}

void ARuneSpirit::PlayDeadAnimation()
{
	Super::PlayDeadAnimation();
}

void ARuneSpirit::PlayHitAnimation()
{
	Super::PlayHitAnimation();
}
