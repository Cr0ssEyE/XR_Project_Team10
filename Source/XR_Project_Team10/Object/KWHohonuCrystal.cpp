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

	CreateNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CreateVFX"));
	
	DropDownNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DropDownVFX"));

	WaveNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WaveVFX"));
	
	DestroyNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DestroyVFX"));

	RootComponent = StaticMeshComponent;
	CreateNiagaraComponent->SetupAttachment(RootComponent);
	DropDownNiagaraComponent->SetupAttachment(RootComponent);
	WaveNiagaraComponent->SetupAttachment(RootComponent);
	DestroyNiagaraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKWHohonuCrystal::BeginPlay()
{
	Super::BeginPlay();
	SC_Hp = BossHohonuDataAsset->SC_Hp;
	CurrentHp = SC_Hp;
	
	SC_DropDownDamage = BossHohonuDataAsset->SC_DropDownDamage;
	SC_DropDownSpeed = BossHohonuDataAsset->SC_DropDownSpeed;
	
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
		SetDeActivate();
	}

	return Result;
}

void AKWHohonuCrystal::ActivateAndDropDownSequence()
{
	CurrentHp = SC_Hp;
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
	CreateNiagaraComponent->Deactivate();
	DropDownNiagaraComponent->Deactivate();
	WaveNiagaraComponent->Deactivate();
	DestroyNiagaraComponent->Deactivate();
	bIsDamageCaused = false;
}

