// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Object/KWHohonuCrystal.h"

#include "Engine/DamageEvents.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
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

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	
	SummonVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CreateVFX"));
	
	DropDownVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DropDownVFX"));

	WaveVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WaveVFX"));
	
	DestroyVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DestroyVFX"));
	
	StaticMeshComponent->SetupAttachment(RootComponent);
	CollisionCapsule->SetupAttachment(RootComponent);
	SummonVFX->SetupAttachment(RootComponent);
	DropDownVFX->SetupAttachment(RootComponent);
	WaveVFX->SetupAttachment(RootComponent);
	DestroyVFX->SetupAttachment(RootComponent);

	StaticMeshComponent->SetStaticMesh(BossHohonuDataAsset->SC_Mesh);
	SummonVFX->SetAsset(BossHohonuDataAsset->SC_SummonVFX);
	DropDownVFX->SetAsset(BossHohonuDataAsset->SC_DropDownVFX);
	WaveVFX->SetAsset(BossHohonuDataAsset->SC_WaveVFX);
	DestroyVFX->SetAsset(BossHohonuDataAsset->SC_DestroyVFX);
}

// Called when the game starts or when spawned
void AKWHohonuCrystal::BeginPlay()
{
	Super::BeginPlay();

	CollisionCapsule->SetCapsuleSize(40.f, 50.f);
	CollisionCapsule->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
	CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	SC_Hp = BossHohonuDataAsset->SC_Hp;
	CurrentHp = SC_Hp;
	bIsDebugEnable = BossHohonuDataAsset->bIsDebugEnable;
	SC_DropDownDamage = BossHohonuDataAsset->SC_DropDownDamage;
	SC_DropDownSpeed = BossHohonuDataAsset->SC_DropDownSpeed;
	SC_DropDownDelay = BossHohonuDataAsset->SC_DropDownDelay;
	
	SC_WaveLength = BossHohonuDataAsset->SC_WaveLength;
	SC_WaveDamage = BossHohonuDataAsset->SC_WaveDamage;

	SC_AttackDelay = BossHohonuDataAsset->SC_AttackDelay;
	SC_MaxAttackRange = BossHohonuDataAsset->SC_MaxAttackRange;
	SC_IncreaseAttackRange = BossHohonuDataAsset->SC_IncreaseAttackRange;
	DisableDestroyVFXTime = 3.f;
	
	SetDeActivate();
	// ActivateAndDropDownSequence();
}

// Called every frame
void AKWHohonuCrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKWHohonuCrystal::Destroyed()
{
	Super::Destroyed();
	GetWorldTimerManager().ClearTimer(DestroyEventTimerHandle);
	GetWorldTimerManager().ClearTimer(DropDownTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveAttackDelayTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveAttackHitCheckTimerHandle);
}

void AKWHohonuCrystal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if(!bIsPlaceInGround && !bIsDropDownDamageCaused)
	{
		AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(OtherActor);
		if(PlayerCharacter)
		{
			//TODO: 나중에 매직넘버 수정하기
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("낙하 도중 플레이어 충돌")));
			FDamageEvent DamageEvent;
			PlayerCharacter->TakeDamage(SC_DropDownDamage, DamageEvent, GetController(), this);
			bIsDropDownDamageCaused = true;
			FVector PlayerDirection = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f , %f"), PlayerDirection.X, PlayerDirection.Y));

			// 높이 체크 필요
			if(GetActorLocation().Z > PlayerCharacter->GetActorLocation().Z)
			{
				PlayerDirection.Z *= -1.f;
			}
			
			ReBoundVector = PlayerDirection * 1000.f;
			PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
		}
	}
}

float AKWHohonuCrystal::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHp -= DamageAmount;
	if(CurrentHp <= 0)
	{
		DestroyVFX->Activate(true);
		SetDeActivate();
		GetWorldTimerManager().SetTimer(DestroyEventTimerHandle, this, &AKWHohonuCrystal::DisableDestroyVFX,DisableDestroyVFXTime, false);
	}
	return Result;
}

void AKWHohonuCrystal::ActivateAndDropDownSequence()
{
	if(bIsActivate)
	{
		FDamageEvent DamageEvent;
		TakeDamage(CurrentHp, DamageEvent, GetController(), this);
		bIsPlaceInGround = false;
		if(GetWorldTimerManager().IsTimerActive(WaveAttackHitCheckTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(WaveAttackHitCheckTimerHandle);
		}
	}
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bIsActivate =true;
	bIsDropDownDamageCaused = false;
	CurrentHp = SC_Hp;
	StaticMeshComponent->SetVisibility(true);
	SummonVFX->Activate(true);
	GetWorldTimerManager().SetTimer(DropDownTimerHandle, this, &AKWHohonuCrystal::DropDownSequence, 0.01f, false, SC_DropDownDelay);
}

void AKWHohonuCrystal::DropDownSequence()
{
	if(GetWorldTimerManager().IsTimerActive(DestroyEventTimerHandle))
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &AKWHohonuCrystal::DropDownSequence);
		return;
	}
	
	if(bIsPlaceInGround)
	{
		return;
	}
	
	SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, -SC_DropDownSpeed * 0.01f));
			
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	FVector BoxCollision = FVector(1.f, 1.f, 1.f);

			
	// 크리스탈 정중앙으로 바닥에 고정되는 위치 조정하기
	bool bResult = GetWorld()->SweepSingleByChannel(
	HitResult,
	GetActorLocation(),
	GetActorLocation(),
	FQuat::Identity,
	ECollisionChannel::ECC_WorldStatic,
	FCollisionShape::MakeBox(BoxCollision),
	Params);
	if(bIsDebugEnable)
	{
		DrawDebugBox(GetWorld(), GetActorLocation(), BoxCollision, FColor::Red);
	}

	if(bResult)
	{
		AKWPlayerCharacter* Player = Cast<AKWPlayerCharacter>(HitResult.GetActor());
		AKWHohonuCrystal* SpawnedCrystal = Cast<AKWHohonuCrystal>(HitResult.GetActor());
		if(Player || SpawnedCrystal)
		{
			return;
		}
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("지면 충돌")));
		bIsPlaceInGround = true;
		DropDownVFX->Activate(true);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SC_CurrentAttackRange = 0;
		GetWorldTimerManager().SetTimer(WaveAttackHitCheckTimerHandle, this, &AKWHohonuCrystal::ActivateWaveAttack, 0.01f, true);
		return;
	}
	GetWorldTimerManager().SetTimerForNextTick(this, &AKWHohonuCrystal::DropDownSequence);
}

void AKWHohonuCrystal::ActivateWaveAttack()
{
	if(GetWorldTimerManager().IsTimerActive(WaveAttackDelayTimerHandle))
	{
		return;
	}
	if(!bIsActivate)
	{
		GetWorldTimerManager().ClearTimer(WaveAttackHitCheckTimerHandle);
	}
	
	TArray<FHitResult> HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
	ECC_PLAYER_ONLY,
	FCollisionShape::MakeSphere(SC_CurrentAttackRange),
	Params);
	if(bIsDebugEnable)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), SC_CurrentAttackRange, 32, FColor::Yellow, false, 0.1f);
	}

	if(bResult && !bIsWaveDamageCaused)
	{
		for (auto Result : HitResult)
		{
			AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(Result.GetActor());
			if(PlayerCharacter)
			{
				// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("파동 범위 내 플레이어 감지")));
				FVector TargetLocation = PlayerCharacter->GetActorLocation();
				FVector XYDistance = GetActorLocation() - TargetLocation;
				XYDistance.Z = 0.f;
				
				if(XYDistance.Length() >= SC_CurrentAttackRange - SC_WaveLength && TargetLocation.Z < GetActorLocation().Z + 80.f)
				{
					if(!bIsActivate)
					{
						GetWorldTimerManager().ClearTimer(WaveAttackHitCheckTimerHandle);
					}
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("파동 끝에 플레이어 충돌 확인")));
					FDamageEvent DamageEvent;
					PlayerCharacter->TakeDamage(SC_WaveDamage, DamageEvent, GetController(), this);
					bIsWaveDamageCaused = true;
					//TODO: 나중에 매직넘버 처리
					FVector PlayerDirection = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
					PlayerDirection.Z = 10.f;
					ReBoundVector = PlayerDirection * 100.f;
					PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
				}
			}
		}
	}
	SC_CurrentAttackRange += SC_IncreaseAttackRange * 0.01f;
	if(SC_CurrentAttackRange >= SC_MaxAttackRange)
	{
		SC_CurrentAttackRange = 0;
		GetWorldTimerManager().SetTimer(WaveAttackDelayTimerHandle, this, &AKWHohonuCrystal::ActivateWaveAttackTimer, SC_AttackDelay, false);
	}
}

void AKWHohonuCrystal::ActivateWaveAttackTimer()
{
	if(!bIsActivate)
	{
		return;
	}
	
	WaveVFX->Activate(true);
	bIsWaveDamageCaused = false;
}

void AKWHohonuCrystal::DisableDestroyVFX()
{
	DestroyVFX->Deactivate();
}

void AKWHohonuCrystal::SetDeActivate()
{
	StaticMeshComponent->SetVisibility(false);
	SummonVFX->Deactivate();
	DropDownVFX->Deactivate();
	WaveVFX->Deactivate();
	bIsActivate = false;
	bIsDropDownDamageCaused = false;
	bIsWaveDamageCaused = false;
	bIsAttackOnGoing = false;
	bIsPlaceInGround = false;
}

