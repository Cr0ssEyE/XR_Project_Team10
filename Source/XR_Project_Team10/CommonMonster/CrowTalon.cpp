#include "XR_Project_Team10/CommonMonster/CrowTalon.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"

ACrowTalon::ACrowTalon()
{
	static ConstructorHelpers::FObjectFinder<UDataAsset> DataAsset(TEXT("/Game/Rolling-Kiwi/Datas/DataAssets/CrowTalon"));
	if (DataAsset.Succeeded()) {
		UDataAsset* dataAsset = DataAsset.Object;
		MonsterData = Cast<UCommonMonsterDataAsset>(dataAsset);
	}
}

void ACrowTalon::BeginPlay()
{
	Super::BeginPlay();

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
	if (nullptr != PlayerTarget) {
		AttackDir = (PlayerTarget->GetActorLocation() - OriPos).GetSafeNormal() * RushSpeed;

		GetWorldTimerManager().SetTimer(RushTimerHandle, this, &ACrowTalon::AttackEndCheck, 0.01f, true);

		AddActorLocalOffset(AttackDir);
	}
}

void ACrowTalon::AttackEndCheck() {
	SetActorLocation(GetActorLocation() + AttackDir);
	if (FVector::Dist(GetActorLocation(), OriPos) >= AttackRange) {
		OnAttackFinished.ExecuteIfBound();
		MonsterState = EState::E_IDLE;
		PlayerTarget = nullptr;
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

}
