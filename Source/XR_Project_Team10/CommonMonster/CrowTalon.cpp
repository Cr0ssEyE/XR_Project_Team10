#include "XR_Project_Team10/CommonMonster/CrowTalon.h"

#include "Engine/DamageEvents.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"

ACrowTalon::ACrowTalon()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/1-Graphic-Resource/Monster/Monster_2/death/NormalMonster2_Dead"));
	if (DeadMontageRef.Object) {
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataAsset> DataAsset(TEXT("/Game/Rolling-Kiwi/Datas/DataAssets/CrowTalon"));
	if (DataAsset.Succeeded()) {
		UDataAsset* dataAsset = DataAsset.Object;
		MonsterData = Cast<UCommonMonsterDataAsset>(dataAsset);
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

	OriPos = GetActorLocation();
}

void ACrowTalon::Attack()
{
	Super::Attack();
	if (nullptr != PlayerTarget)
	{
		// AttackDir = (PlayerTarget->GetActorLocation() - OriPos).GetSafeNormal() * RushSpeed;
		AttackDir = GetActorForwardVector() * RushSpeed;
		GetWorldTimerManager().SetTimer(RushTimerHandle, this, &ACrowTalon::AttackEndCheck, 0.01f, true);

		AddActorLocalOffset(AttackDir);
	}
}

void ACrowTalon::AttackEndCheck()
{
	SetActorLocation(GetActorLocation() + AttackDir);
	
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	bool bResult = GetWorld()->SweepSingleByChannel(
	HitResult,
	GetActorLocation(),
	GetActorLocation(),
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
				MonsterState = EState::E_IDLE;
				PlayerTarget = nullptr;
				bIsDamageCaused = true;
				
				FDamageEvent DamageEvent;
				PlayerCharacter->TakeDamage(RushPower, DamageEvent, nullptr, this);
				
				FVector PlayerDirection = PlayerLocation->GetActorLocation() - GetActorLocation();
				//TODO: 매직넘버 수정하기
				ReBoundVector = PlayerDirection * 10.f;
				ReBoundVector.Z = 3000.f;
				PlayerCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Enemy);
				
				GetWorldTimerManager().ClearTimer(RushTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(RushTimerHandle);
			}
		}
	}
	if (FVector::Dist(GetActorLocation(), OriPos) >= AttackRange)
	{
		OnAttackFinished.ExecuteIfBound();
		MonsterState = EState::E_IDLE;
		PlayerTarget = nullptr;
		bIsDamageCaused = false;
		GetWorldTimerManager().ClearTimer(RushTimerHandle);
		FPPTimerHelper::InvalidateTimerHandle(RushTimerHandle);
	}
}

void ACrowTalon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	OnAttackFinished.ExecuteIfBound();
	MonsterState = EState::E_IDLE;
	PlayerTarget = nullptr;
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

