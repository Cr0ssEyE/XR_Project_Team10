// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Object/KWHohonuCrystal.h"

#include "Engine/DamageEvents.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"

// Sets default values
AKWHohonuCrystal::AKWHohonuCrystal()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BossHohonuDataAsset = FPPConstructorHelper::FindAndGetObject<UKWBossHohonuDataAsset>(TEXT("/Script/XR_Project_Team10.KWBossHohonuDataAsset'/Game/21-Hohonu/Datas/Hohonu_DataAsset.Hohonu_DataAsset'"));
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalMesh"));

	SummonVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CreateVFX"));
	
	DropDownVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DropDownVFX"));

	WaveVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WaveVFX"));
	
	DestroyVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DestroyVFX"));

	RootComponent = StaticMeshComponent;
	SummonVFX->SetupAttachment(RootComponent);
	DropDownVFX->SetupAttachment(RootComponent);
	WaveVFX->SetupAttachment(RootComponent);
	DestroyVFX->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKWHohonuCrystal::BeginPlay()
{
	Super::BeginPlay();
	SC_Hp = BossHohonuDataAsset->SC_Hp;
	CurrentHp = SC_Hp;
	
	SC_DropDownDamage = BossHohonuDataAsset->SC_DropDownDamage;
	SC_DropDownSpeed = BossHohonuDataAsset->SC_DropDownSpeed;
	SC_DropDownDelay = BossHohonuDataAsset->SC_DropDownDelay;
	
	SC_WaveLength = BossHohonuDataAsset->SC_WaveLength;
	SC_WaveDamage = BossHohonuDataAsset->SC_WaveDamage;

	SC_AttackDelay = BossHohonuDataAsset->SC_AttackDelay;
	SC_MaxAttackRange = BossHohonuDataAsset->SC_MaxAttackRange;
	SC_IncreaseAttackRangePerSecond = BossHohonuDataAsset->SC_IncreaseAttackRangePerSecond;
	
	SetDeActivate();
}

// Called every frame
void AKWHohonuCrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AKWHohonuCrystal::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHp -= DamageAmount;
	if(CurrentHp <= 0)
	{
		DestroyVFX->Activate();
		GetWorldTimerManager().SetTimer(DestroyEventTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if(FPPTimerHelper::IsDelayElapsed(DestroyEventTimerHandle, 1.f))
			{
				SetDeActivate();
				GetWorldTimerManager().ClearTimer(DestroyEventTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(DestroyEventTimerHandle);
			}
		}),0.01f, true);
	}
	return Result;
}

void AKWHohonuCrystal::ActivateAndDropDownSequence()
{
	CurrentHp = SC_Hp;
	SummonVFX->Activate(true);
	GetWorldTimerManager().SetTimer(DropDownTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(FPPTimerHelper::IsDelayElapsed(DropDownTimerHandle, SC_DropDownDelay) && bIsSpawnDelayOnGoing)
		{
			bIsSpawnDelayOnGoing = false;
		}

		if(bIsSpawnDelayOnGoing)
		{
			return;
		}
		
		if(FPPTimerHelper::IsDelayElapsed(DropDownTimerHandle, 0.01f) && !bIsPlaceInGround)
		{
			SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, SC_DropDownSpeed));
			
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);

			FVector BoxCollision = FVector(10.f, 10.f, 10.f);

			// 크리스탈 정중앙으로 충돌체크 위치 조정하기
			bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_EngineTraceChannel1,
			FCollisionShape::MakeBox(BoxCollision),
			Params);
			DrawDebugBox(GetWorld(), GetActorLocation(), BoxCollision, FColor::Red);

			if(bResult)
			{
				bIsPlaceInGround = true;
				GetWorldTimerManager().ClearTimer(DropDownTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(DropDownTimerHandle);
			}

			TArray<FHitResult> PlayerHitResult;
			
			// 낙하 함수에서 체크하는 방법 vs 콜리전 컴포넌트 만들고 오버랩 이벤트에서 체크하는 방법 고민 중
			// 추후 크리스탈 사이즈에 맞춰 충돌 처리 범위 조절해야 함
			bResult = GetWorld()->SweepMultiByChannel(
			PlayerHitResult,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_Pawn,
			FCollisionShape::MakeCapsule(45.f, 90.f),
			Params);

			if(bResult)
			{
				for (auto Result : PlayerHitResult)
				{
					AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(Result.GetActor());
					if(PlayerCharacter)
					{
						FDamageEvent DamageEvent;
						PlayerCharacter->TakeDamage(SC_DropDownDamage, DamageEvent, GetController(), this);
						
						FVector PlayerDirection = (GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal();
						PlayerDirection.Z = -10.f;
						FVector ReBoundVector = PlayerDirection * -100.f;
						
						PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
					}
				}
			}
		}
		
	}), 0.01f, true);
	
}

void AKWHohonuCrystal::ActivateWaveAttack()
{
	if(GetWorldTimerManager().IsTimerActive(WaveAttackDelayTimerHandle) || bIsDamageCaused)
	{
		return;
	}
	SC_CurrentAttackRange = 0;
	GetWorldTimerManager().SetTimer(WaveAttackHitCheckTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(GetWorldTimerManager().IsTimerActive(WaveAttackDelayTimerHandle) || bIsDamageCaused)
		{
			return;
		}
		
		if(FPPTimerHelper::IsDelayElapsed(WaveAttackHitCheckTimerHandle, 0.01f))
		{
			TArray<FHitResult> HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);
			
			bool bResult = GetWorld()->SweepMultiByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_Pawn,
			FCollisionShape::MakeSphere(SC_CurrentAttackRange),
			Params);
			DrawDebugSphere(GetWorld(), GetActorLocation(), SC_CurrentAttackRange, 32, FColor::Yellow, false, 0.5f);
			
			if(bResult)
			{
				for (auto Result : HitResult)
				{
					AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(Result.GetActor());
					if(PlayerCharacter)
					{
						FVector TargetLocation = PlayerCharacter->GetActorLocation();
						FVector XYDistance = GetActorLocation() - TargetLocation;
						XYDistance.Z = 0.f;
						
						if(XYDistance.Length() >= SC_CurrentAttackRange - SC_WaveLength && TargetLocation.Z - 10.f < GetActorLocation().Z)
						{
							FDamageEvent DamageEvent;
							PlayerCharacter->TakeDamage(SC_WaveDamage, DamageEvent, GetController(), this);
							bIsDamageCaused = true;

							FVector PlayerDirection = (GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal();
							PlayerDirection.Z = -10.f;
							FVector ReBoundVector = PlayerDirection * -100.f;
						
							PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
						}
					}
				}
			}
			SC_CurrentAttackRange += SC_IncreaseAttackRangePerSecond * 0.01f;
			if(SC_CurrentAttackRange >= SC_MaxAttackRange)
			{
				SC_CurrentAttackRange = 0;
				ActivateWaveAttackTimer();
			}
		}
	}), 0.01f, true);
}

void AKWHohonuCrystal::ActivateWaveAttackTimer()
{
	GetWorldTimerManager().SetTimer(WaveAttackDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(FPPTimerHelper::IsDelayElapsed(WaveAttackDelayTimerHandle, SC_AttackDelay))
		{
			GetWorldTimerManager().ClearTimer(WaveAttackDelayTimerHandle);
			FPPTimerHelper::InvalidateTimerHandle(WaveAttackDelayTimerHandle);
			bIsDamageCaused = false;
		}
	}), 0.01f, true);
}

void AKWHohonuCrystal::SetDeActivate()
{
	StaticMeshComponent->SetVisibility(false);
	SummonVFX->Deactivate();
	DropDownVFX->Deactivate();
	WaveVFX->Deactivate();
	DestroyVFX->Deactivate();
	bIsDamageCaused = false;
	bIsAttackOnGoing = false;
	bIsPlaceInGround = false;
	bIsSpawnDelayOnGoing = true;
}

