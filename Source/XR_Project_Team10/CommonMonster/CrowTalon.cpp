#include "XR_Project_Team10/CommonMonster/CrowTalon.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"

ACrowTalon::ACrowTalon()
{static ConstructorHelpers::FObjectFinder<UDataAsset> DataAsset(TEXT("/Game/Rolling-Kiwi/Datas/DataAssets/CrowTalon"));
	if (DataAsset.Succeeded()) {
		UDataAsset* dataAsset = DataAsset.Object;
		MonsterData = Cast<UCommonMonsterDataAsset>(dataAsset);
	}
}

void ACrowTalon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACrowTalon::AttackOmen(AActor* Target)
{
	UE_LOG(LogTemp, Log, TEXT("CrowTalon Attack Omen"));

	OriPos = GetOwner()->GetActorLocation();
}

void ACrowTalon::Attack(AActor* Target)
{
	FVector AttackDir = Target->GetActorLocation().GetSafeNormal();

	RushVector = MonsterStaticMesh->GetPhysicsLinearVelocity().GetSafeNormal() * RushSpeed;
	RushVector.Z = 0.f;
	if (RushVector.Length() < 100.f)
	{
		RushVector = FVector(100.f, 100.f, 0.f);
	}

	GetWorldTimerManager().SetTimer(RushTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
				if (FVector::Dist(GetOwner()->GetActorLocation(), OriPos) >= AttackRange) {
					GetWorldTimerManager().ClearTimer(RushTimerHandle);
					FPPTimerHelper::InvalidateTimerHandle(RushTimerHandle);
				}
		}), 0.01f, true);

	MonsterStaticMesh->SetPhysicsLinearVelocity(RushVector);
}

void ACrowTalon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

