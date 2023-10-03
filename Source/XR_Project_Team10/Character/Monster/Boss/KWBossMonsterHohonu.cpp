// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"

#include "KWBossAnimDataAsset.h"
#include "KWBossHohonuAnimInstance.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossHohonuDataAsset.h"
#include "NiagaraComponent.h"

AKWBossMonsterHohonu::AKWBossMonsterHohonu()
{
	HohonuRingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HohonuRing"));
	HohonuHeadEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HohonuHead"));
	HohonuRingEffect->SetupAttachment(GetMesh());
	HohonuHeadEffect->SetupAttachment(GetMesh());
	
	UKWBossHohonuDataAsset* HohonuData = Cast<UKWBossHohonuDataAsset>(BossMonsterStatusData);
	if(HohonuData)
	{
		GetMesh()->SetSkeletalMesh(HohonuData->HohonuMesh);
		HohonuRingEffect->SetAsset(HohonuData->HohonuRingEffect);
		HohonuHeadEffect->SetAsset(HohonuData->HohonuHeadEffect);
		// 나중에 소켓 달아서 이펙트 위지 지정하기
	}
	
	UKWBossAnimDataAsset* BossAnimData = Cast<UKWBossAnimDataAsset>(BossMonsterAnimData);
	if(BossAnimData)
	{
		GetMesh()->SetAnimInstanceClass(BossAnimData->BossAnimBlueprint->GetAnimBlueprintGeneratedClass());
		BossAnimMontage = BossAnimData->BossAnimMontage;
		UKWBossHohonuAnimInstance* HohonuAnimInstance = Cast<UKWBossHohonuAnimInstance>(GetMesh()->GetAnimInstance());
		if(HohonuAnimInstance)
		{
			HohonuAnimInstance->OmenPatternDelegate.AddUObject(this, &AKWBossMonsterHohonu::ActivatePatternOmen);
			HohonuAnimInstance->PatternToggleDelegate.AddUObject(this, &AKWBossMonsterHohonu::TogglePattern);
		}
	}
}

void AKWBossMonsterHohonu::BeginPlay()
{
	Super::BeginPlay();
	InitData();
}

void AKWBossMonsterHohonu::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AKWBossMonsterHohonu::InitData()
{
	Super::InitData();
	UKWBossHohonuDataAsset* HohonuData = Cast<UKWBossHohonuDataAsset>(BossMonsterStatusData);
	if(HohonuData)
	{
		BossHp = HohonuData->HohonuHp;
		HohonuLunaticHp = HohonuData->HohonuLunaticHp;
		MoveSpeed - HohonuData->HohonuMoveSpeed;
		
		SC_Count = HohonuData->SC_Count;
		SC_Speed = HohonuData->SC_Speed;

		SL_Damage = HohonuData->SL_Damage;
		SL_AttackDelay = HohonuData->SL_AttackDelay;
		SL_Degree = HohonuData->SL_Degree;
		SL_TurnSpeed = HohonuData->SL_TurnSpeed;
		SL_AttackRange = HohonuData->SL_AttackRange;
		
		MA_Damage = HohonuData->MA_Damage;
		MA_AttackSpeed = HohonuData->MA_AttackSpeed;
		
		WW_Damage = HohonuData->WW_Damage;
		WW_AttackDelay = HohonuData->WW_AttackDelay;
		WW_AttackTime = HohonuData->WW_AttackTime;
		WW_IncreaseMoveSpeed = HohonuData->WW_IncreaseMoveSpeed;
		WW_MaxMoveSpeed = HohonuData->WW_MaxMoveSpeed;
		
		BS_Range = HohonuData->BS_Range;
		BS_MoveSpeed = HohonuData->BS_MoveSpeed;
		
		ML_Damage = HohonuData->ML_Damage;
		ML_AttackTime = HohonuData->ML_AttackTime;
		ML_TurnSpeed = HohonuData->ML_TurnSpeed;
	}
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

void AKWBossMonsterHohonu::ActivatePatternOmen(EHohonuPattern Pattern)
{
	switch (Pattern)
	{
	case EHohonuPattern::SummonCrystal:
		OmenPattern_SC();
		break;
	case EHohonuPattern::SweepLaser:
		OmenPattern_SL();
		break;
	case EHohonuPattern::MeleeAttack:
		OmenPattern_MA();
		break;
	case EHohonuPattern::WhirlWind:
		OmenPattern_WW();
		break;
	case EHohonuPattern::BackStep:
		OmenPattern_BS();
		break;
	case EHohonuPattern::MultipleLaser:
		OmenPattern_ML();
		break;
	default:
		checkNoEntry();
	}
}

void AKWBossMonsterHohonu::TogglePattern(EHohonuPattern Pattern, bool Value)
{
	switch (Pattern)
	{
	case EHohonuPattern::SummonCrystal:
		ExecutePattern_SC(Value);
		break;
	case EHohonuPattern::SweepLaser:
		ExecutePattern_SL(Value);
		break;
	case EHohonuPattern::MeleeAttack:
		ExecutePattern_MA(Value);
		break;
	case EHohonuPattern::WhirlWind:
		ExecutePattern_WW(Value);
		break;
	case EHohonuPattern::BackStep:
		ExecutePattern_BS(Value);
		break;
	case EHohonuPattern::MultipleLaser:
		ExecutePattern_ML(Value);
		break;
	default:
		checkNoEntry();
	}
}

void AKWBossMonsterHohonu::OmenPattern_SC()
{
	
}

void AKWBossMonsterHohonu::OmenPattern_SL()
{
	
}

void AKWBossMonsterHohonu::OmenPattern_MA()
{
	
}

void AKWBossMonsterHohonu::OmenPattern_WW()
{
	
}

void AKWBossMonsterHohonu::OmenPattern_BS()
{
	
}

void AKWBossMonsterHohonu::OmenPattern_ML()
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_SC(bool Value)
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_SL(bool Value)
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_MA(bool Value)
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_WW(bool Value)
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_BS(bool Value)
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_ML(bool Value)
{
	
}
