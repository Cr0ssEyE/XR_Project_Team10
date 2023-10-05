// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"

#include "KWBossAnimDataAsset.h"
#include "KWBossHohonuAnimInstance.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossHohonuDataAsset.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"

AKWBossMonsterHohonu::AKWBossMonsterHohonu()
{
	BossMonsterStatusData = FPPConstructorHelper::FindAndGetObject<UDataAsset>(TEXT("/Script/XR_Project_Team10.KWBossHohonuDataAsset'/Game/21-Hohonu/Datas/Hohonu_DataAsset.Hohonu_DataAsset'"));

	BossMonsterAnimData = FPPConstructorHelper::FindAndGetObject<UDataAsset>(TEXT("/Script/XR_Project_Team10.KWBossAnimDataAsset'/Game/21-Hohonu/Datas/Hohonu_AnimDataAsset.Hohonu_AnimDataAsset'"));

	BossMonsterAIData = FPPConstructorHelper::FindAndGetObject<UDataAsset>(TEXT("/Script/XR_Project_Team10.KWBossHohonuAIDataAsset'/Game/21-Hohonu/Datas/Hohonu_AIDataAsset.Hohonu_AIDataAsset'"));

	SetActorScale3D(FVector::OneVector * 2);
	GetCapsuleComponent()->SetCapsuleSize(150.f, 200.f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -150.f));
	
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
		// 나중에 소켓 달아서 이펙트 위치 지정하기
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
			HohonuAnimInstance->PatternToggleDelegate.AddUObject(this, &AKWBossMonsterHohonu::ActivatePatternExecute);
		}
	}
}

void AKWBossMonsterHohonu::BeginPlay()
{
	Super::BeginPlay();
	InitData();
	bIsPatternRunning = false;
}

void AKWBossMonsterHohonu::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// 테스트용
	if(!bIsPatternRunning)
	{
		TargetPlayer = CastChecked<AKWPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		bIsPatternRunning = true;
		ExecutePattern_WW();
	}
}

void AKWBossMonsterHohonu::InitData()
{
	Super::InitData();
	UKWBossHohonuDataAsset* HohonuData = Cast<UKWBossHohonuDataAsset>(BossMonsterStatusData);
	if(HohonuData)
	{
		BossHp = HohonuData->HohonuHp;
		
		HohonuLunaticHp = HohonuData->HohonuLunaticHp;
		MoveSpeed = HohonuData->HohonuMoveSpeed;
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		
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
		WW_IncreaseMoveSpeedPerSecond = HohonuData->WW_IncreaseMoveSpeed;
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

void AKWBossMonsterHohonu::ActivatePatternOmen(const EHohonuPattern Pattern)
{
	bIsPatternRunning = true;
	switch (Pattern)
	{
	case EHohonuPattern::SummonCrystal:
		CurrentPattern = EHohonuPattern::SummonCrystal;
		OmenPattern_SC();
		break;
	case EHohonuPattern::SweepLaser:
		CurrentPattern = EHohonuPattern::SweepLaser;
		OmenPattern_SL();
		break;
	case EHohonuPattern::MeleeAttack:
		CurrentPattern = EHohonuPattern::MeleeAttack;
		OmenPattern_MA();
		break;
	case EHohonuPattern::WhirlWind:
		CurrentPattern = EHohonuPattern::WhirlWind;
		OmenPattern_WW();
		break;
	case EHohonuPattern::BackStep:
		CurrentPattern = EHohonuPattern::BackStep;
		OmenPattern_BS();
		break;
	case EHohonuPattern::MultipleLaser:
		CurrentPattern = EHohonuPattern::MultipleLaser;
		OmenPattern_ML();
		break;
	default:
		checkNoEntry();
	}
}

void AKWBossMonsterHohonu::ActivatePatternExecute(const EHohonuPattern Pattern)
{
	if(bIsPatternRunning)
	{
		bIsPatternRunning = false;
		CurrentPattern = EHohonuPattern::None;
		UE_LOG(LogTemp, Log, TEXT("Hohonu Pattern End"));
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Hohonu Pattern Start"));
	switch (Pattern)
	{
	case EHohonuPattern::SummonCrystal:
		ExecutePattern_SC();
		break;
	case EHohonuPattern::SweepLaser:
		ExecutePattern_SL();
		break;
	case EHohonuPattern::MeleeAttack:
		ExecutePattern_MA();
		break;
	case EHohonuPattern::WhirlWind:
		ExecutePattern_WW();
		break;
	case EHohonuPattern::BackStep:
		ExecutePattern_BS();
		break;
	case EHohonuPattern::MultipleLaser:
		ExecutePattern_ML();
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

void AKWBossMonsterHohonu::ExecutePattern_SC()
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_SL()
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_MA()
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_WW()
{
	UE_LOG(LogTemp, Log, TEXT("Hohonu WhirlWind Start"));
	// 훨윈드
	GetWorldTimerManager().SetTimer(WW_TimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(!bIsPatternRunning)
		{
			GetWorldTimerManager().ClearTimer(WW_TimerHandle);
			FPPTimerHelper::InvalidateTimerHandle(WW_TimerHandle);
		}
		if(FPPTimerHelper::IsDelayElapsed(WW_TimerHandle, 0.01f))
		{
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);
			
			bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_Pawn,
			FCollisionShape::MakeBox(FVector(400.f, 400.f, 400.f)),
			Params);
			
			if(bResult)
			{
				AKWPlayerCharacter* Player = Cast<AKWPlayerCharacter>(HitResult.GetActor());
				if(Player)
				{
					FDamageEvent DamageEvent;
					Player->TakeDamage(WW_Damage, DamageEvent, GetController(), this);
				}
			}

			FVector MoveDirection = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddActorLocalRotation(FRotator(0.f, 10.f, 0.f));
			AddMovementInput(MoveDirection);
			/*
			if(GetCharacterMovement()->MaxWalkSpeed < WW_MaxMoveSpeed)
			{
				GetCharacterMovement()->MaxWalkSpeed += WW_IncreaseMoveSpeedPerSecond * FPPTimerHelper::GetActualDeltaTime(WW_TimerHandle);
				if(GetCharacterMovement()->MaxWalkSpeed > WW_MaxMoveSpeed)
				{
					GetCharacterMovement()->MaxWalkSpeed = WW_MaxMoveSpeed;
				}
			}
			*/
		}
	}), 0.01f, true);
	/*
	while (true)
	{
		if(!bIsPatternRunning)
		{
			break;
		}
		GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]()
		{
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);
			
			bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_Pawn,
			FCollisionShape::MakeBox(FVector(400.f, 400.f, 400.f)),
			Params);
			
			if(bResult)
			{
				AKWPlayerCharacter* Player = Cast<AKWPlayerCharacter>(HitResult.GetActor());
				if(Player)
				{
					FDamageEvent DamageEvent;
					Player->TakeDamage(WW_Damage, DamageEvent, GetController(), this);
				}
			}

			FVector MoveDirection = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(MoveDirection);
			if(GetCharacterMovement()->MaxWalkSpeed < WW_MaxMoveSpeed)
			{
				GetCharacterMovement()->MaxWalkSpeed += WW_IncreaseMoveSpeed;
				if(GetCharacterMovement()->MaxWalkSpeed > WW_MaxMoveSpeed)
				{
					GetCharacterMovement()->MaxWalkSpeed = WW_MaxMoveSpeed;
				}
			}
		}));
	}
	*/
}

void AKWBossMonsterHohonu::ExecutePattern_BS()
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_ML()
{
	
}
