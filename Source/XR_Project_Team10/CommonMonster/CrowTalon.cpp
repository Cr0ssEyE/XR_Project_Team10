#include "XR_Project_Team10/CommonMonster/CrowTalon.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"
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

	UE_LOG(LogTemp, Log, TEXT("CrowTalon BeginPlay"));

}

void ACrowTalon::AttackOmen()
{
	Super::AttackOmen();
	UE_LOG(LogTemp, Log, TEXT("CrowTalon Attack Omen"));

}

void ACrowTalon::Attack()
{
	float TurnSpeed = 5.f;
	FVector LookVector = PlayerTarget->GetActorLocation() - GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	OriPos = GetActorLocation();

	Super::Attack();
}

void ACrowTalon::AttackBehaviour()
{
	AttackDir = (PlayerTarget->GetActorLocation() - OriPos).GetSafeNormal() * RushSpeed;

	GetWorldTimerManager().SetTimer(RushTimerHandle, this, &ACrowTalon::AttackEndCheck, 0.01f, true);

	AddActorLocalOffset(AttackDir);
}

void ACrowTalon::AttackEnd()
{
	Super::AttackEnd();
}

void ACrowTalon::AttackEndCheck() {
	SetActorLocation(GetActorLocation() + AttackDir);
	if (FVector::Dist(GetActorLocation(), OriPos) >= AttackRange) {
		GetWorldTimerManager().ClearTimer(RushTimerHandle);
		FPPTimerHelper::InvalidateTimerHandle(RushTimerHandle);
	}
}

void ACrowTalon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	OnAttackFinished.ExecuteIfBound();
}

void ACrowTalon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

