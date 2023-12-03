// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/CommonMonster/RuneSpirit.h"

#include "Engine/DamageEvents.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
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

float ARuneSpirit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if(GetController())
	{
		GetController()->Destroy();
	}
	CommonMonsterDead();
	return 0;
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
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
		
	bool bResult = GetWorld()->SweepSingleByChannel(
	HitResult,
	GetActorLocation(),
	GetActorLocation() + GetActorForwardVector() * 200.f,
	FQuat::Identity,
	ECC_PLAYER_ONLY,
	FCollisionShape::MakeBox(FVector(100.f, 100.f, 100.f)),
	Params);
	if(bResult)
	{
		AKWLocationDetector* PlayerLocation = Cast<AKWLocationDetector>(HitResult.GetActor());
		if(PlayerLocation)
		{
			AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(PlayerLocation->GetTargetCharacter());
			if(PlayerCharacter)
			{
				OnAttackFinished.ExecuteIfBound();
				PlayerTarget = nullptr;

				FDamageEvent DamageEvent;
				PlayerCharacter->TakeDamage(1, DamageEvent, nullptr, this);
				FVector PlayerDirection = PlayerLocation->GetActorLocation() - GetActorLocation();
				ReBoundVector = PlayerDirection * 10.f;
				ReBoundVector.Z = 3000.f;
				PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
			}
		}
	}
}

void ARuneSpirit::PlayDeadAnimation()
{
	Super::PlayDeadAnimation();
}

void ARuneSpirit::PlayHitAnimation()
{
	Super::PlayHitAnimation();
}
