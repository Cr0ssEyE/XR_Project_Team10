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

void ACrowTalon::AttackOmen(AActor* Target)
{
	UE_LOG(LogTemp, Log, TEXT("CrowTalon Attack Omen"));

	OriPos = GetActorLocation();
}

FVector AttackDir;
void ACrowTalon::Attack(AActor* Target)
{
	AttackDir = (Target->GetActorLocation() - OriPos).GetSafeNormal() * RushSpeed;

	GetWorldTimerManager().SetTimer(RushTimerHandle, this, &ACrowTalon::AttackEndCheck, 0.01f, true);

	//AddActorLocalOffset(AttackDir);
	//AddMovementInput(AttackDir);

	//MonsterComponent->SetPhysicsLinearVelocity(AttackDir * RushSpeed);
	//UE_LOG(LogTemp, Log, TEXT("%f %f %f"), MonsterComponent->GetPhysicsLinearVelocity().X, MonsterComponent->GetPhysicsLinearVelocity().Y, MonsterComponent->GetPhysicsLinearVelocity().Z);
	//MonsterComponent->AddForce(AttackDir * RushSpeed);
}

void ACrowTalon::AttackEndCheck() {
	SetActorLocation(GetActorLocation() + AttackDir);
	//AddMovementInput(AttackDir);
	if (FVector::Dist(GetActorLocation(), OriPos) >= AttackRange) {
		UE_LOG(LogTemp, Log, TEXT("dash end"));
		OnAttackFinished.ExecuteIfBound();
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

