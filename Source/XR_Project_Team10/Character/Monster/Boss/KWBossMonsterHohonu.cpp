// Fill out your copyright notice in the Description page of Project Settings.

#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossHohonuAnimInstance.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossHohonuDataAsset.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossAnimDataAsset.h"
#include "XR_Project_Team10/AI/Boss/Hohonu/KWHohonuAIController.h"
#include "XR_Project_Team10/Constant/KWAnimMontageSectionName.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Constant/KWMeshSocketName.h"

AKWBossMonsterHohonu::AKWBossMonsterHohonu()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AKWHohonuAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
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

	HohonuLeftHandEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HohonuLeftHand"));
	HohonuRightHandEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HohonuRightHand"));
	HohonuLeftHandEffect->SetupAttachment(GetMesh());
	HohonuRightHandEffect->SetupAttachment(GetMesh());
	
	UKWBossHohonuDataAsset* HohonuData = Cast<UKWBossHohonuDataAsset>(BossMonsterStatusData);
	if(HohonuData)
	{
		GetMesh()->SetSkeletalMesh(HohonuData->HohonuMesh);
		HohonuRingEffect->SetAsset(HohonuData->HohonuRingEffect);
		HohonuHeadEffect->SetAsset(HohonuData->HohonuHeadEffect);
	}
	
	UKWBossAnimDataAsset* BossAnimData = Cast<UKWBossAnimDataAsset>(BossMonsterAnimData);
	if(BossAnimData)
	{
		GetMesh()->SetAnimInstanceClass(BossAnimData->BossAnimBlueprint->GetAnimBlueprintGeneratedClass());
		BossAnimMontage = BossAnimData->BossAnimMontage;
	}
}

void AKWBossMonsterHohonu::BeginPlay()
{
	Super::BeginPlay();
	InitData();
	GetMesh()->SetCollisionEnabled(::ECollisionEnabled::NoCollision);
	HohonuAnimInstance = CastChecked<UKWBossHohonuAnimInstance>(GetMesh()->GetAnimInstance());
	if(HohonuAnimInstance)
	{
		HohonuAnimInstance->OnMontageStarted.AddDynamic(this, &AKWBossMonsterHohonu::ActivatePatternOmen);
		HohonuAnimInstance->OnMontageEnded.AddDynamic(this, &AKWBossMonsterHohonu::FinishAIPatternNode);
		HohonuAnimInstance->PatternActivateDelegate.AddUObject(this, &AKWBossMonsterHohonu::ActivatePatternExecute);
		HohonuAnimInstance->PatternDeActivateDelegate.AddUObject(this, &AKWBossMonsterHohonu::StopPattern);
	}
	
	AKWHohonuAIController* AIController = Cast<AKWHohonuAIController>(GetController());
	if(AIController)
	{
		// 인카운터 모션 나오면 적용 예정
		// AIController->DeActivateAI();
		// GetMesh()->GetAnimInstance()->Montage_JumpToSection(SECTION_ENCOUNTER, BossAnimMontage);
	}
	bIsPatternRunning = false;
}

void AKWBossMonsterHohonu::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	HohonuRingEffect->SetWorldLocation(GetMesh()->GetSocketLocation(HOHONU_VFX_RING));
	HohonuRingEffect->SetWorldRotation(GetMesh()->GetSocketRotation(HOHONU_VFX_RING));
	HohonuHeadEffect->SetWorldLocation(GetMesh()->GetSocketLocation(HOHONU_VFX_HEAD));
	HohonuHeadEffect->SetWorldRotation(GetMesh()->GetSocketRotation(HOHONU_VFX_HEAD));
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
		
		SC_SpawnDelay = HohonuData->SC_SpawnDelay;
		SC_SpawnHeight = HohonuData->SC_SpawnHeight;
		
		SL_Damage = HohonuData->SL_Damage;
		SL_AttackDelay = HohonuData->SL_AttackDelay;
		SL_Degree = HohonuData->SL_Degree;
		SL_TurnSpeed = HohonuData->SL_TurnSpeed;
		SL_AttackRange = HohonuData->SL_AttackRange;
		
		MA_Damage = HohonuData->MA_Damage;
		MA_DamageRange = HohonuData->MA_DamageRange;
		MA_ExplodeDamageRange = HohonuData->MA_ExplodeDamageRange;
		
		WW_Damage = HohonuData->WW_Damage;
		WW_DamageRange = HohonuData->WW_DamageRange;
		WW_AttackDelay = HohonuData->WW_AttackDelay;
		WW_AttackTime = HohonuData->WW_AttackTime;
		WW_IncreaseMoveSpeedPerSecond = HohonuData->WW_IncreaseMoveSpeed;
		WW_MaxMoveSpeed = HohonuData->WW_MaxMoveSpeed;
		WW_RotateSpeed = HohonuData->WW_RotateSpeed;
		
		BS_Range = HohonuData->BS_Time;
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

void AKWBossMonsterHohonu::SetAIPatternDelegate(const FAICharacterPatternFinished& PatternFinishedDelegate)
{
	Super::SetAIPatternDelegate(PatternFinishedDelegate);
	CharacterPatternFinished = PatternFinishedDelegate;
}

void AKWBossMonsterHohonu::EndEncounterAnimation()
{
	Super::EndEncounterAnimation();
	AKWHohonuAIController* AIController = Cast<AKWHohonuAIController>(GetController());
	if(AIController)
	{
		AIController->ActivateAI();
	}
}

void AKWBossMonsterHohonu::PlayPatternAnimMontage()
{
	Super::PlayPatternAnimMontage();
	if(bIsPatternRunning)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("패턴 진행중 패턴 실행 노드 진입 오류 발생")));
		return;
	}
	
	GetMesh()->GetAnimInstance()->Montage_Play(BossAnimMontage);
	
	switch (CurrentPattern)
	{
	case EHohonuPattern::SummonCrystal:
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SECTION_SUMMON_CRYSTAL, BossAnimMontage);
		break;
	case EHohonuPattern::SweepLaser:
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SECTION_SWEEP_LASER, BossAnimMontage);
		break;
	case EHohonuPattern::MeleeAttack:
		HohonuLeftHandEffect->Activate();
		HohonuRightHandEffect->Activate();
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SECTION_MELEE_ATTACK, BossAnimMontage);
		break;
	case EHohonuPattern::WhirlWind:
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SECTION_WHIRL_WIND, BossAnimMontage);
		break;
	case EHohonuPattern::BackStep:
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SECTION_BACK_STEP, BossAnimMontage);
		break;
	case EHohonuPattern::MultipleLaser:
		// 애니메이션 나오면 추가
		break;
	default:
		checkNoEntry();
	}
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

void AKWBossMonsterHohonu::ActivatePatternOmen(UAnimMontage* Montage)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("패턴 전조 실행")));
	switch (CurrentPattern)
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

void AKWBossMonsterHohonu::ActivatePatternExecute(const EHohonuPattern Pattern)
{
	if(bIsPatternRunning && Pattern != EHohonuPattern::MeleeAttack)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("패턴 진행중 패턴 실행 노드 진입 오류 발생")));
		UE_LOG(LogTemp, Log, TEXT("Hohonu Pattern Still Running"));
		return;
	}
	bIsPatternRunning = true;
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

void AKWBossMonsterHohonu::FinishAIPatternNode(UAnimMontage* Montage, bool IsInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("패턴 노드 종료")));
	bIsPatternRunning = false;
	CharacterPatternFinished.Broadcast();
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
	if(!TargetPlayer)
	{
		return;
	}
	
	if(!SC_Instances.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Crystal Instance Blank!!! "));
		return;
	}
	
	SC_SpawnCount = 0;
	UE_LOG(LogTemp, Log, TEXT("Hohonu SummonCrystal Start"));
	bIsAttacking = true;

	if(SC_SpawnCount < SC_Instances.Num())
	{
		SC_Instances[SC_SpawnCount]->SetActorLocation(FVector(TargetPlayer->GetActorLocation().X, TargetPlayer->GetActorLocation().Y, SC_SpawnHeight));
		SC_Instances[SC_SpawnCount]->ActivateAndDropDownSequence();
		SC_SpawnCount++;
	}
	GetWorldTimerManager().SetTimer(SC_SpawnTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(SC_SpawnCount < SC_Instances.Num())
		{
			SC_Instances[SC_SpawnCount]->ActivateAndDropDownSequence();
			SC_Instances[SC_SpawnCount]->SetActorLocation(FVector(TargetPlayer->GetActorLocation().X, TargetPlayer->GetActorLocation().Y, SC_SpawnHeight));
			SC_SpawnCount++;
			if(SC_SpawnCount == SC_Instances.Num())
			{
				SC_SpawnCount = 0;
				GetWorldTimerManager().ClearTimer(SC_SpawnTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(SC_SpawnTimerHandle);
			}
		}
	}), SC_SpawnDelay, true);
	bIsAttacking = false;
}

void AKWBossMonsterHohonu::ExecutePattern_SL()
{
	
}

void AKWBossMonsterHohonu::ExecutePattern_MA()
{
	UE_LOG(LogTemp, Log, TEXT("Hohonu MeleeAttack Start"));
	bIsAttacking = true;
	bIsMeleeAttackDamageCaused = false;
	GetWorldTimerManager().SetTimer(MA_TimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(!bIsPatternRunning)
		{
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);
			bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetMesh()->GetSocketLocation(HOHONU_VFX_FRONT),
			GetMesh()->GetSocketLocation(HOHONU_VFX_FRONT),
			FQuat::Identity,
			ECC_PLAYER_ONLY,
			FCollisionShape::MakeBox(MA_ExplodeDamageRange),
			Params);
			DrawDebugBox(GetWorld(), GetMesh()->GetSocketLocation(HOHONU_VFX_FRONT), MA_ExplodeDamageRange, FColor::Blue, false, 0.3f);
			if(bResult)
			{
				AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(HitResult.GetActor());
				if(PlayerCharacter)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("플레이어 충돌")));
					FDamageEvent DamageEvent;
					PlayerCharacter->TakeDamage(MA_Damage, DamageEvent, GetController(), this);
					FVector PlayerDirection = PlayerCharacter->GetActorLocation() - GetActorLocation();
					PlayerDirection.Z = 100.f;
					ReBoundVector = PlayerDirection * 10.f;
					PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
				}
			}
			GetWorldTimerManager().ClearTimer(MA_TimerHandle);
			FPPTimerHelper::InvalidateTimerHandle(MA_TimerHandle);
		}
		FHitResult HitResultL;
		FHitResult HitResultR;
		FCollisionQueryParams Params(NAME_None, false, this);

		HohonuLeftHandEffect->SetWorldLocation(GetMesh()->GetSocketLocation(HOHONU_HAND_LEFT));
		bool bResultL = GetWorld()->SweepSingleByChannel(
		HitResultL,
		GetMesh()->GetSocketLocation(HOHONU_HAND_LEFT),
		GetMesh()->GetSocketLocation(HOHONU_HAND_LEFT),
		FQuat::Identity,
		ECC_PLAYER_ONLY,
		FCollisionShape::MakeBox(MA_DamageRange),
		Params);
		DrawDebugBox(GetWorld(), GetMesh()->GetSocketLocation(HOHONU_HAND_LEFT), MA_DamageRange, FColor::Red, false, 0.1f);

		HohonuRightHandEffect->SetWorldLocation(GetMesh()->GetSocketLocation(HOHONU_HAND_RIGHT));
		bool bResultR = GetWorld()->SweepSingleByChannel(
		HitResultR,
		GetMesh()->GetSocketLocation(HOHONU_HAND_RIGHT),
		GetMesh()->GetSocketLocation(HOHONU_HAND_RIGHT),
		FQuat::Identity,
		ECC_PLAYER_ONLY,
		FCollisionShape::MakeBox(MA_DamageRange),
		Params);
		DrawDebugBox(GetWorld(), GetMesh()->GetSocketLocation(HOHONU_HAND_RIGHT), MA_DamageRange, FColor::Red, false, 0.1f);
		
		if(bResultL || bResultR)
		{
			AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(HitResultL.GetActor());
			if(!PlayerCharacter)
			{
				PlayerCharacter = Cast<AKWPlayerCharacter>(HitResultR.GetActor());
			}
			if(PlayerCharacter && !bIsMeleeAttackDamageCaused)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("플레이어 충돌")));
				bIsMeleeAttackDamageCaused = true;
				FDamageEvent DamageEvent;
				PlayerCharacter->TakeDamage(MA_Damage, DamageEvent, GetController(), this);
				FVector PlayerDirection = PlayerCharacter->GetActorLocation() - GetActorLocation();
				PlayerDirection.Z = 100.f;
				ReBoundVector = PlayerDirection * 10.f;
				PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
			}
		}
	}), 0.01f, true);
}

void AKWBossMonsterHohonu::ExecutePattern_WW()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("훨윈드 시작")));
	bIsAttacking = true;
	bIsWhirlWindDamageCaused = false;
	// 훨윈드
	GetWorldTimerManager().SetTimer(WW_TimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(!bIsPatternRunning)
		{
			bIsAttacking = false;
			GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
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
			ECC_PLAYER_ONLY,
			FCollisionShape::MakeBox(WW_DamageRange),
			Params);

			DrawDebugBox(GetWorld(), GetActorLocation(), WW_DamageRange, FColor::Red);
			
			if(bResult)
			{
				AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(HitResult.GetActor());
				if(PlayerCharacter && !bIsWhirlWindDamageCaused)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("플레이어 충돌")));
					bIsWhirlWindDamageCaused = true;
					FDamageEvent DamageEvent;
					PlayerCharacter->TakeDamage(WW_Damage, DamageEvent, GetController(), this);
					FVector PlayerDirection = PlayerCharacter->GetActorLocation() - GetActorLocation();
					ReBoundVector = PlayerDirection * 10.f;
					ReBoundVector.Z = 1000.f;
					PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
				}
			}
			FVector MoveDirection = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddActorLocalRotation(FRotator(0.f, WW_RotateSpeed * 0.01f, 0.f));
			AddMovementInput(MoveDirection);
			if(GetCharacterMovement()->MaxWalkSpeed < WW_MaxMoveSpeed)
			{
				GetCharacterMovement()->MaxWalkSpeed += WW_IncreaseMoveSpeedPerSecond * FPPTimerHelper::GetActualDeltaTime(WW_TimerHandle);
				if(GetCharacterMovement()->MaxWalkSpeed > WW_MaxMoveSpeed)
				{
					GetCharacterMovement()->MaxWalkSpeed = WW_MaxMoveSpeed;
				}
			}
		}
	}), 0.01f, true);
}

void AKWBossMonsterHohonu::ExecutePattern_BS()
{
	UE_LOG(LogTemp, Log, TEXT("Hohonu Backstep Start"));
	GetWorldTimerManager().SetTimer(BackStepTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(FPPTimerHelper::IsDelayElapsed(BackStepTimerHandle, BS_Range))
		{
			GetWorldTimerManager().ClearTimer(BackStepTimerHandle);
			FPPTimerHelper::InvalidateTimerHandle(BackStepTimerHandle);
		}
		if(FPPTimerHelper::IsDelayElapsed(BackStepTimerHandle, 0.01f))
		{
			SetActorLocation( GetActorLocation() + GetActorForwardVector() * - BS_MoveSpeed * 0.01f);
		}
	}), 0.01f, true);
}

void AKWBossMonsterHohonu::ExecutePattern_ML()
{
	
}
