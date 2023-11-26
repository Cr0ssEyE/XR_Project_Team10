#include "XR_Project_Team10/CommonMonster/CrowTalon.h"

#include "Engine/DamageEvents.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

ACrowTalon::ACrowTalon()
{
	DeadMontage = FPPConstructorHelper::FindAndGetObject<UAnimMontage>(TEXT("/Script/Engine.AnimMontage'/Game/1-Graphic-Resource/Monster/CrowTalon/Animation/AM_CrowTalon_Dead.AM_CrowTalon_Dead'"));
	HitMontage = FPPConstructorHelper::FindAndGetObject<UAnimMontage>(TEXT("/Script/Engine.AnimMontage'/Game/1-Graphic-Resource/Monster/CrowTalon/Animation/AM_CrowTalon_Hit.AM_CrowTalon_Hit'"));

	MonsterData = FPPConstructorHelper::FindAndGetObject<UCommonMonsterDataAsset>(TEXT("/Game/Rolling-Kiwi/Datas/DataAssets/CrowTalon"));
}

void ACrowTalon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(MonsterState == EState::E_ATTACK_OMEN)
	{
		if(PlayerTarget)
		{
			FVector LockVector = (PlayerTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			RootComponent->SetRelativeRotation(LockVector.Rotation());
		}
	}
}

void ACrowTalon::BeginPlay()
{
	Super::BeginPlay();
	bIsDamageCaused = false;
	UE_LOG(LogTemp, Log, TEXT("CrowTalon BeginPlay"));

}

void ACrowTalon::AttackOmen()
{
	Super::AttackOmen();
	UE_LOG(LogTemp, Log, TEXT("CrowTalon Attack Omen"));

}

void ACrowTalon::Attack()
{
	Super::Attack();
	if(PlayerTarget)
	{
		FVector LookVector = PlayerTarget->GetActorLocation() - GetActorLocation();
		float TurnSpeed = 5.f;
		LookVector.Z = 0.0f;
		FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));
		OriPos = GetActorLocation();
	}
}

void ACrowTalon::AttackBehaviour()
{
	if (nullptr != PlayerTarget)
	{
		AttackDir = (PlayerTarget->GetActorLocation() - OriPos).GetSafeNormal() * RushSpeed;
		AttackDir = GetActorForwardVector() * RushSpeed;
		MonsterAttackElapsedTime = 0;
		OriPos = GetActorLocation();
		GetWorldTimerManager().SetTimerForNextTick(this, &ACrowTalon::AttackEndCheck);
	}
	AddActorLocalOffset(AttackDir);
}

void ACrowTalon::AttackEnd()
{
	Super::AttackEnd();
}

void ACrowTalon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	OnAttackFinished.ExecuteIfBound();
	MonsterState = EState::E_IDLE;
	PlayerTarget = nullptr;
}

void ACrowTalon::AttackEndCheck()
{
	MonsterAttackElapsedTime += GetWorld()->DeltaTimeSeconds;
	if(MonsterAttackElapsedTime >= MonsterAttackTime || MonsterState == EState::E_ATTACK_END)
	{
		GetWorldTimerManager().ClearTimer(RushTimerHandle);
		FPPTimerHelper::InvalidateTimerHandle(RushTimerHandle);
		return;
	}
	SetActorLocation(GetActorLocation() + AttackDir);
	
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
	// DrawDebugBox(GetWorld(), GetActorLocation() + GetActorForwardVector() * 200.f, FVector(100.f, 100.f, 100.f), FColor::Blue, false, 0.3f);
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
				bIsDamageCaused = true;
					
				FDamageEvent DamageEvent;
				PlayerCharacter->TakeDamage(RushPower, DamageEvent, nullptr, this);
					
				// FVector PlayerDirection = PlayerLocation->GetActorLocation() - GetActorLocation();
				// TODO: 매직넘버 수정하기
				// ReBoundVector = PlayerDirection * 10.f;
				// ReBoundVector.Z = 3000.f;
				// PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
					
				GetWorldTimerManager().ClearTimer(RushTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(RushTimerHandle);
				return;
			}
		}
	}
	
	if (FVector::Dist(GetActorLocation(), OriPos) >= AttackRange)
    {
    	OnAttackFinished.ExecuteIfBound();
    	MonsterState = EState::E_ATTACK_END;
    	PlayerTarget = nullptr;
    	bIsDamageCaused = false;
    	GetWorldTimerManager().ClearTimer(RushTimerHandle);
    	FPPTimerHelper::InvalidateTimerHandle(RushTimerHandle);
    }
	GetWorldTimerManager().SetTimerForNextTick(this, &ACrowTalon::AttackEndCheck);
}

void ACrowTalon::PlayDeadAnimation()
{
	Super::PlayDeadAnimation();
}

void ACrowTalon::PlayHitAnimation()
{
	Super::PlayHitAnimation();
}


