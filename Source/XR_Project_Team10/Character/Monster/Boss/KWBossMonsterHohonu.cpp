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
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Constant/KWMeshSocketName.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"

AKWBossMonsterHohonu::AKWBossMonsterHohonu()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AKWHohonuAIController::StaticClass();
	// AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	BossMonsterStatusData = FPPConstructorHelper::FindAndGetObject<UDataAsset>(TEXT("/Script/XR_Project_Team10.KWBossHohonuDataAsset'/Game/Rolling-Kiwi/Datas/DataAssets/Hohonu_DataAsset.Hohonu_DataAsset'"));

	BossMonsterAnimData = FPPConstructorHelper::FindAndGetObject<UDataAsset>(TEXT("/Script/XR_Project_Team10.KWBossAnimDataAsset'/Game/Rolling-Kiwi/Datas/DataAssets/Hohonu_AnimDataAsset.Hohonu_AnimDataAsset'"));

	BossMonsterAIData = FPPConstructorHelper::FindAndGetObject<UDataAsset>(TEXT("/Script/XR_Project_Team10.KWBossHohonuAIDataAsset'/Game/Rolling-Kiwi/Datas/DataAssets/Hohonu_AIDataAsset.Hohonu_AIDataAsset'"));

	SetActorScale3D(FVector::OneVector * 1.5);
	GetCapsuleComponent()->SetCapsuleSize(100.f, 50.f);

	HitCheckBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCheckBox"));
	HitCheckBoxComponent->SetupAttachment(GetMesh());
	HitCheckBoxComponent->SetCanEverAffectNavigation(false);
	HitCheckBoxComponent->bDynamicObstacle = true;
	HitCheckBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HitCheckBoxComponent->SetCollisionProfileName(CP_ENEMY);
	HitCheckBoxComponent->SetCollisionObjectType(ECC_ENEMY);
	
	HohonuRingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HohonuRingVFX"));
	HohonuHeadEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HohonuHeadVFX"));
	HohonuRingEffect->SetupAttachment(GetMesh());
	HohonuHeadEffect->SetupAttachment(GetMesh());
	
	HohonuLaserSweepEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HohonuLaserSweepVFX"));
	HohonuLaserSweepEffect->SetupAttachment(GetMesh());

	HohonuLaserBurnEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HohonuLaserBurnVFX"));
	HohonuLaserBurnEffect->SetupAttachment(GetMesh());
	HohonuLaserBurnEffect->SetRelativeScale3D(FVector3d::One() * 2);
	
	UKWBossHohonuDataAsset* HohonuData = Cast<UKWBossHohonuDataAsset>(BossMonsterStatusData);
	if(HohonuData)
	{
		GetMesh()->SetSkeletalMesh(HohonuData->HohonuMesh);
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		HohonuRingEffect->SetAsset(HohonuData->HohonuRingEffect);
		HohonuHeadEffect->SetAsset(HohonuData->HohonuHeadEffect);
		HohonuLaserSweepEffect->SetAsset(HohonuData->SL_LaserVFX);
		HohonuLaserBurnEffect->SetAsset(HohonuData->SL_BurnVFX);
	}
	
	UKWBossAnimDataAsset* BossAnimData = Cast<UKWBossAnimDataAsset>(BossMonsterAnimData);
	if(BossAnimData)
	{
		GetMesh()->SetAnimInstanceClass(FPPConstructorHelper::FindAndGetClass<UKWBossHohonuAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/1-Graphic-Resource/Monster/Boss/Animation/ABP_Boss_Hohonu.ABP_Boss_Hohonu_C'"), EAssertionLevel::Check));
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
		HohonuAnimInstance->OnMontageEnded.AddDynamic(this, &AKWBossMonsterHohonu::FinishAIPatternNode);
		HohonuAnimInstance->PatternActivateDelegate.AddUObject(this, &AKWBossMonsterHohonu::ActivatePatternExecute);
		HohonuAnimInstance->PatternDeActivateDelegate.AddUObject(this, &AKWBossMonsterHohonu::StopPattern);
	}

	HohonuLaserSweepEffect->Deactivate();
	HohonuLaserBurnEffect->Deactivate();
	SL_OriginRotation = HohonuLaserSweepEffect->GetRelativeRotation();
	AKWHohonuAIController* AIController = Cast<AKWHohonuAIController>(GetController());
	if(AIController)
	{
		AIController->DeActivateAI();
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
	if(bIsDebugEnable)
	{
		DrawDebugBox(GetWorld(), GetActorLocation(), HitCheckBoxComponent->GetScaledBoxExtent(), FColor::Magenta, false, 0.3f);
	}
}

void AKWBossMonsterHohonu::InitData()
{
	Super::InitData();
	UKWBossHohonuDataAsset* HohonuData = Cast<UKWBossHohonuDataAsset>(BossMonsterStatusData);
	if(HohonuData)
	{
		HitCheckBoxComponent->SetBoxExtent(HohonuData->HohonuHitBoxCollision);
		HitCheckBoxComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

		HitKnockBackMultiplyValue = HohonuData->HitKnockBackMultiplyValue;
		HitKnockBackHeightValue = HohonuData->HitKnockBackHeightValue;
		
		BossHp = HohonuData->HohonuHp;
		bIsDebugEnable = HohonuData->bIsDebugEnable;
		
		HohonuLunaticHp = HohonuData->HohonuLunaticHp;
		MoveSpeed = HohonuData->HohonuMoveSpeed;
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		
		SC_SpawnDelay = HohonuData->SC_SpawnDelay;
		SC_SpawnHeight = HohonuData->SC_SpawnHeight;

		bSL_BIsRandomStart = HohonuData->SL_bIsRandomStart;
		SL_Damage = HohonuData->SL_Damage;
		SL_Degree = HohonuData->SL_Degree;
		SL_ActiveTime = HohonuData->SL_ActiveTime;
		SL_Distance = HohonuData->SL_Distance;
		SL_DamageRange = HohonuData->SL_DamageRange;
		SL_KnockBackMultiplyValue = HohonuData->SL_KnockBackMultiplyValue;
		SL_KnockBackHeightValue = HohonuData->SL_KnockBackHeightValue;
		
		MA_Damage = HohonuData->MA_Damage;
		MA_DamageRange = HohonuData->MA_DamageRange;
		MA_ExplodeDamageRange = HohonuData->MA_ExplodeDamageRange;
		MA_KnockBackMultiplyValue = HohonuData->MA_KnockBackMultiplyValue;
		MA_KnockBackHeightValue = HohonuData->MA_KnockBackHeightValue;
		
		WW_Damage = HohonuData->WW_Damage;
		WW_DamageRange = HohonuData->WW_DamageRange;
		WW_IncreaseMoveSpeedPerSecond = HohonuData->WW_IncreaseMoveSpeed;
		WW_MaxMoveSpeed = HohonuData->WW_MaxMoveSpeed;
		WW_RotateSpeed = HohonuData->WW_RotateSpeed;
		WW_KnockBackMultiplyValue = HohonuData->WW_KnockBackMultiplyValue;
		WW_KnockBackHeightValue = HohonuData->WW_KnockBackHeightValue;
		
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
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	BossHp -= DamageAmount;

	if(BossHp <= 0)
	{
		if(GetController())
		{
			GetController()->Destroy();
		}
	}
	
	AKWPlayerCharacter* Causer = Cast<AKWPlayerCharacter>(DamageCauser);
	if(Causer)
	{
		AActor* PlayerCharacterLocation = Causer->GetTruePlayerLocation();
		FVector PlayerDirection = PlayerCharacterLocation->GetActorLocation() - GetActorLocation();
		ReBoundVector = PlayerDirection * HitKnockBackMultiplyValue;
		ReBoundVector.Z = HitKnockBackHeightValue;
		Causer->RB_ApplyKnockBackByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
	}
	return 0;
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
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("수정 소환 시작")));
		OmenPattern_SC();
		break;
	case EHohonuPattern::SweepLaser:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("레이저 공격 시작")));
		OmenPattern_SL();
		break;
	case EHohonuPattern::MeleeAttack:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("근접 공격 시작")));
		bIsMeleeAttackDamageCaused = false;
		GetWorldTimerManager().SetTimerForNextTick(this, &AKWBossMonsterHohonu::ExecutePattern_MA);
		break;
	case EHohonuPattern::WhirlWind:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("훨윈드 시작")));
		bIsWhirlWindDamageCaused = false;
		GetWorldTimerManager().SetTimerForNextTick(this, &AKWBossMonsterHohonu::ExecutePattern_WW);
		break;
	case EHohonuPattern::BackStep:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("백스텝 시작")));
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

void AKWBossMonsterHohonu::ActivateAI()
{
	AKWHohonuAIController* AIController = Cast<AKWHohonuAIController>(GetController());
	if(AIController)
	{
		AIController->ActivateAI();
	}
}

void AKWBossMonsterHohonu::OmenPattern_SC()
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

	SC_Instances[SC_SpawnCount]->SetActorLocation(FVector(TargetPlayer->GetActorLocation().X, TargetPlayer->GetActorLocation().Y, TargetPlayer->GetActorLocation().Z + SC_SpawnHeight));
	SC_Instances[SC_SpawnCount]->ActivateAndDropDownSequence();
	SC_SpawnCount++;
	
	GetWorldTimerManager().SetTimer(SC_SpawnTimerHandle, this, &AKWBossMonsterHohonu::ExecutePattern_SC, SC_SpawnDelay, true);
}

void AKWBossMonsterHohonu::OmenPattern_SL()
{
	UE_LOG(LogTemp, Log, TEXT("Hohonu SweepLaser Start"));
	bIsSweepLaserDamageCaused = false;
	HohonuLaserSweepEffect->Activate();
	HohonuLaserBurnEffect->Activate();
	if(bSL_BIsRandomStart)
	{
		bIsSweepLeftToRight = FMath::RandRange(0, 1);
	}
	else
	{
		bIsSweepLeftToRight = true;
	}
	
	HohonuLaserSweepEffect->SetRelativeRotation(SL_OriginRotation);
	
	if(bIsSweepLeftToRight)
	{
		HohonuLaserSweepEffect->SetRelativeRotation(HohonuLaserSweepEffect->GetComponentRotation() - FRotator(0.f, SL_Degree / 2, 0.f));
	}
	else
	{
		HohonuLaserSweepEffect->SetRelativeRotation(HohonuLaserSweepEffect->GetComponentRotation() + FRotator(0.f, SL_Degree / 2, 0.f));
	}
	SL_ElapsedTime = 0;
	GetWorldTimerManager().SetTimerForNextTick(this, &AKWBossMonsterHohonu::ExecutePattern_SL);
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
	if(SC_SpawnCount < SC_Instances.Num())
	{
		SC_Instances[SC_SpawnCount]->ActivateAndDropDownSequence();
		SC_Instances[SC_SpawnCount]->SetActorLocation(FVector(TargetPlayer->GetActorLocation().X, TargetPlayer->GetActorLocation().Y, TargetPlayer->GetActorLocation().Z + SC_SpawnHeight));
		SC_SpawnCount++;
		if(SC_SpawnCount == SC_Instances.Num())
		{
			SC_SpawnCount = 0;
			GetWorldTimerManager().ClearTimer(SC_SpawnTimerHandle);
		}
	}
}

void AKWBossMonsterHohonu::ExecutePattern_SL()
{

	if(!bIsPatternRunning)
	{
		HohonuLaserSweepEffect->Deactivate();
		HohonuLaserBurnEffect->Deactivate();
		return;
	}
	
	FVector StartLocation = HohonuLaserSweepEffect->GetComponentLocation();
	FVector EndLocation = StartLocation + HohonuLaserSweepEffect->GetUpVector() * SL_Distance;

	FHitResult HitResult;
	FCollisionQueryParams Param(NAME_None, false, this);
	bool bGroundResult = GetWorld()->LineTraceSingleByProfile(
	HitResult,
	HohonuLaserSweepEffect->GetRelativeLocation(),
	HohonuLaserSweepEffect->GetRelativeLocation() + HohonuLaserSweepEffect->GetUpVector() * 10000,
	CP_STATIC_ONLY,
	Param);
	//DrawDebugBox(GetWorld(), HohonuLaserSweepEffect->GetRelativeLocation() + HohonuLaserSweepEffect->GetUpVector() * 500, FVector3d::One() * 30, FColor::Blue, false, 0.3f);
	if(bGroundResult)
	{
		HohonuLaserBurnEffect->SetWorldLocation(FVector(EndLocation.X, EndLocation.Y, HitResult.Location.Z + 10.f));
	}
	else
	{
		HohonuLaserBurnEffect->SetWorldLocation(FVector(EndLocation.X, EndLocation.Y, HitCheckBoxComponent->GetComponentLocation().Z - HitCheckBoxComponent->GetScaledBoxExtent().Z));
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Ground: %f, %f"), EndLocation.X, EndLocation.Y));
	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepMultiByChannel(
	HitResults,
	StartLocation,
	EndLocation,
	FQuat::Identity,
	ECC_PLAYER_ONLY,
	FCollisionShape::MakeBox(SL_DamageRange),
	Params);
	
	if(bIsDebugEnable)
	{
		for(float i = 0; i <= 1;)
		{
			float DrawCenterX = FMath::Lerp<float, float>(StartLocation.X, EndLocation.X, i);
			float DrawCenterY = FMath::Lerp<float, float>(StartLocation.Y, EndLocation.Y, i);
			float DrawCenterZ = FMath::Lerp<float, float>(StartLocation.Z, EndLocation.Z, i);
			FVector DrawCenter = FVector(DrawCenterX, DrawCenterY, DrawCenterZ);
			DrawDebugBox(GetWorld(), DrawCenter, SL_DamageRange, FColor::Blue, false, 0.3f);
			i += 0.1;
		}
	}
	
	if(bResult && !bIsSweepLaserDamageCaused)
	{
		for (auto Result : HitResults)
		{
			AKWLocationDetector* PlayerCharacterLocation = Cast<AKWLocationDetector>(Result.GetActor());
			if(PlayerCharacterLocation)
			{
				AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(PlayerCharacterLocation->GetTargetCharacter());
				if(PlayerCharacter)
				{
					//TODO:: 매직넘버 처리하기
					bIsSweepLaserDamageCaused = true;
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("플레이어 충돌")));
					FDamageEvent DamageEvent;
					PlayerCharacter->TakeDamage(SL_Damage, DamageEvent, GetController(), this);
					FVector PlayerDirection = PlayerCharacterLocation->GetActorLocation() - GetActorLocation();
					ReBoundVector = PlayerDirection * SL_KnockBackMultiplyValue;
					ReBoundVector.Z = SL_KnockBackHeightValue;
					PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
				}
			}
		}
	}
	SL_ElapsedTime += GetWorld()->DeltaTimeSeconds;
	float RotateValue = SL_Degree * (SL_ElapsedTime / SL_ActiveTime) / 2;
	if(SL_ElapsedTime >= SL_ActiveTime && RotateValue >= SL_Degree)
	{
		HohonuLaserSweepEffect->Deactivate();
		HohonuLaserBurnEffect->Deactivate();
		return;
	}
	
	if(bIsSweepLeftToRight)
	{
		// HohonuLaserSweepEffect->SetRelativeRotation(SL_OriginRotation + FRotator(0.f, RotateValue, 0.f));
		HohonuLaserSweepEffect->SetRelativeRotation(SL_OriginRotation - FRotator(0.f, SL_Degree / 2, 0.f));
		HohonuLaserSweepEffect->AddRelativeRotation(FRotator(0.f, RotateValue, 0.f));
	}
	else
	{
		// HohonuLaserSweepEffect->SetRelativeRotation(SL_OriginRotation - FRotator(0.f, RotateValue, 0.f));
		HohonuLaserSweepEffect->SetRelativeRotation(SL_OriginRotation - FRotator(0.f, SL_Degree / 2, 0.f));
		HohonuLaserSweepEffect->AddRelativeRotation(FRotator(0.f, -RotateValue, 0.f));
	}
	if(bIsDebugEnable)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("각도: %f"), HohonuLaserSweepEffect->GetComponentRotation().Yaw));
	}
	
	GetWorldTimerManager().SetTimerForNextTick(this, &AKWBossMonsterHohonu::ExecutePattern_SL);
}

void AKWBossMonsterHohonu::ExecutePattern_MA()
{
	if(!bIsPatternRunning)
	{
		TArray<FHitResult> HitResults;
		FCollisionQueryParams Params(NAME_None, false, this);
	
		bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_PLAYER_ONLY,
		FCollisionShape::MakeBox(MA_ExplodeDamageRange),
		Params);
		if(bIsDebugEnable)
		{
			DrawDebugBox(GetWorld(), GetMesh()->GetSocketLocation(HOHONU_VFX_FRONT), MA_ExplodeDamageRange, FColor::Blue, false, 0.3f);
		}
		if(bResult)
		{
			for (auto Result : HitResults)
			{
				AKWLocationDetector* PlayerCharacterLocation = Cast<AKWLocationDetector>(Result.GetActor());
				if(PlayerCharacterLocation)
				{
					AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(PlayerCharacterLocation->GetTargetCharacter());
					if(PlayerCharacter)
					{
						//TODO:: 매직넘버 처리하기
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("플레이어 충돌")));
						FDamageEvent DamageEvent;
						PlayerCharacter->TakeDamage(MA_Damage, DamageEvent, GetController(), this);
						FVector PlayerDirection = PlayerCharacterLocation->GetActorLocation() - GetActorLocation();
						ReBoundVector = PlayerDirection * MA_KnockBackMultiplyValue;
						ReBoundVector.Z = MA_KnockBackHeightValue;
						PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
					}
				}
			}
		}
		return;
	}
	
	TArray<FHitResult> HitResultL;
	TArray<FHitResult> HitResultR;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	bool bResultL = GetWorld()->SweepMultiByChannel(
	HitResultL,
	GetActorLocation(),
	GetActorLocation(),
	FQuat::Identity,
	ECC_PLAYER_ONLY,
	FCollisionShape::MakeBox(MA_DamageRange),
	Params);

	if(bIsDebugEnable)
	{
		DrawDebugBox(GetWorld(), GetMesh()->GetSocketLocation(HOHONU_HAND_LEFT), MA_DamageRange, FColor::Red, false, 0.1f);
	}
	
	bool bResultR = GetWorld()->SweepMultiByChannel(
	HitResultR,
	GetActorLocation(),
	GetActorLocation(),
	FQuat::Identity,
	ECC_PLAYER_ONLY,
	FCollisionShape::MakeBox(MA_DamageRange),
	Params);
	
	if(bIsDebugEnable)
	{
		DrawDebugBox(GetWorld(), GetMesh()->GetSocketLocation(HOHONU_HAND_RIGHT), MA_DamageRange, FColor::Red, false, 0.1f);
	}
	
	if(bResultL || bResultR)
	{
		for(auto Result : HitResultL)
		{
			AKWLocationDetector* PlayerCharacterLocation = Cast<AKWLocationDetector>(Result.GetActor());
			if(PlayerCharacterLocation && !bIsMeleeAttackDamageCaused)
			{
				AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(PlayerCharacterLocation->GetTargetCharacter());
				if(PlayerCharacter)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("플레이어 충돌")));
					bIsMeleeAttackDamageCaused = true;
					FDamageEvent DamageEvent;
					PlayerCharacter->TakeDamage(MA_Damage, DamageEvent, GetController(), this);
					FVector PlayerDirection = PlayerCharacterLocation->GetActorLocation() - GetActorLocation();
					ReBoundVector = PlayerDirection * MA_KnockBackMultiplyValue;
					ReBoundVector.Z = MA_KnockBackHeightValue;
					PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
				}
			}
		}

		for(auto Result : HitResultR)
		{
			AKWLocationDetector* PlayerCharacterLocation = Cast<AKWLocationDetector>(Result.GetActor());
			if(PlayerCharacterLocation && !bIsMeleeAttackDamageCaused)
			{
				AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(PlayerCharacterLocation->GetTargetCharacter());
				if(PlayerCharacter)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("플레이어 충돌")));
					bIsMeleeAttackDamageCaused = true;
					FDamageEvent DamageEvent;
					PlayerCharacter->TakeDamage(MA_Damage, DamageEvent, GetController(), this);
					FVector PlayerDirection = PlayerCharacterLocation->GetActorLocation() - GetActorLocation();
					ReBoundVector = PlayerDirection * MA_KnockBackMultiplyValue;
					ReBoundVector.Z = MA_KnockBackHeightValue;
					PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
				}
			}
		}
	}
	GetWorldTimerManager().SetTimerForNextTick(this, &AKWBossMonsterHohonu::ExecutePattern_MA);
}

void AKWBossMonsterHohonu::ExecutePattern_WW()
{
	if(!bIsPatternRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		return;
	}
	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	bool bResult = GetWorld()->SweepMultiByChannel(
	HitResults,
	GetActorLocation(),
	GetActorLocation(),
	FQuat::Identity,
	ECC_PLAYER_ONLY,
	FCollisionShape::MakeBox(WW_DamageRange),
	Params);

	if(bIsDebugEnable)
	{
		DrawDebugBox(GetWorld(), GetActorLocation(), WW_DamageRange, FColor::Red);
	}
	
	if(bResult)
	{
		for (auto Result : HitResults)
		{
			AKWLocationDetector* PlayerCharacterLocation = Cast<AKWLocationDetector>(Result.GetActor());
			if(PlayerCharacterLocation && !bIsWhirlWindDamageCaused)
			{
				AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(PlayerCharacterLocation->GetTargetCharacter());
				if(PlayerCharacter)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("플레이어 충돌")));
					bIsWhirlWindDamageCaused = true;
					FDamageEvent DamageEvent;
					PlayerCharacter->TakeDamage(WW_Damage, DamageEvent, GetController(), this);
					FVector PlayerDirection = PlayerCharacterLocation->GetActorLocation() - GetActorLocation();
					ReBoundVector = PlayerDirection * WW_KnockBackMultiplyValue;
					ReBoundVector.Z = WW_KnockBackHeightValue;
					PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
				}
			}
		}
	}
	
	if(TargetPlayer)
	{
		FVector MoveDirection = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		AddActorLocalRotation(FRotator(0.f, WW_RotateSpeed * 0.01f, 0.f));

		// TODO: 매직 넘버 수정
		if(FVector::DistXY(GetActorLocation(), TargetPlayer->GetActorLocation()) > 500.f)
		{
			AddMovementInput(MoveDirection);
		}
		if(GetCharacterMovement()->MaxWalkSpeed < WW_MaxMoveSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed += WW_IncreaseMoveSpeedPerSecond * FPPTimerHelper::GetActualDeltaTime(WW_TimerHandle);
			if(GetCharacterMovement()->MaxWalkSpeed > WW_MaxMoveSpeed)
			{
				GetCharacterMovement()->MaxWalkSpeed = WW_MaxMoveSpeed;
			}
		}
	}
	
	GetWorldTimerManager().SetTimerForNextTick(this, &AKWBossMonsterHohonu::ExecutePattern_WW);
}

void AKWBossMonsterHohonu::ExecutePattern_BS()
{
	if(!bIsPatternRunning)
	{
		return;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool ObjectResult = GetWorld()->LineTraceSingleByProfile(
		HitResult,
		GetActorLocation(),
		GetActorLocation() - GetActorForwardVector() * 500.f,
		CP_STATIC_ONLY,
		Params
	);

	if(ObjectResult)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("후방 오브젝트 감지")));
		return;
	}
	
	SetActorLocation( GetActorLocation() + GetActorForwardVector() * - BS_MoveSpeed * 0.01f);
	GetWorldTimerManager().SetTimerForNextTick(this, &AKWBossMonsterHohonu::ExecutePattern_BS);
}

void AKWBossMonsterHohonu::ExecutePattern_ML()
{
	
}
	
