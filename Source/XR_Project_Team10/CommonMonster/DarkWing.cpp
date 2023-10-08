
#include "XR_Project_Team10/CommonMonster/DarkWing.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "XR_Project_Team10/CommonMonster/DW_FeatherProjectile.h"

ADarkWing::ADarkWing()
{
	static ConstructorHelpers::FObjectFinder<UDataAsset> DataAsset(TEXT("/Game/Rolling-Kiwi/Datas/DataAssets/DarkWing"));
	if (DataAsset.Succeeded()) {
		UDataAsset* dataAsset = DataAsset.Object;
		MonsterData = Cast<UCommonMonsterDataAsset>(dataAsset);
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Feather(TEXT("/Game/10-Common-Monster-Base/MyDW_FeatherProjectile"));
		if (Feather.Succeeded()) {
			FeatherClass = Feather.Object->GeneratedClass;
	}

		if (AttackOffsets.Num() == 0) {
			AttackOffsets.Add(FVector(0, 0, 0));
		}
}

void ADarkWing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADarkWing::BeginPlay()
{
	Super::BeginPlay();
}
//
////공격 확인
//void ADarkWing::AttackConfig()
//{
//	FHitResult HitResult;
//	FCollisionQueryParams Params(SCENE_QUERY_STAT(Player), false, this);
//
//	float AttackCheckRange = MonsterData->MonsterAttackConfigRange;
//
//	const FVector Start = GetActorLocation();
//	const FVector End = Start + GetActorForwardVector() * AttackCheckRange;
//
//	bool bResult = GetWorld()->SweepSingleByChannel(
//		HitResult,
//		Start,
//		End,
//		FQuat::Identity,
//		ECollisionChannel::ECC_GameTraceChannel2,
//		FCollisionShape::MakeSphere(AttackCheckRange),
//		Params
//	);
//
//#if ENABLE_DRAW_DEBUG
//	FVector TraceVec = GetActorForwardVector() * AttackCheckRange;
//	FVector Center = GetActorLocation();
//	float HalfHeight = AttackCheckRange * 0.5f + AttackCheckRange;
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//	float DebugLifeTime = 0;
//
//	DrawDebugCapsule
//	(
//		GetWorld(),
//		Center,
//		HalfHeight,
//		AttackCheckRange,
//		CapsuleRot,
//		DrawColor,
//		false,
//		DebugLifeTime
//	);
//#endif
//
//	if (bResult) {
//		UE_LOG(LogTemp, Log, TEXT("%s"), *HitResult.GetActor()->GetName());
//	}
//}

//공격 전조
void ADarkWing::AttackOmen(AActor* Target)
{
	// 전조
	UE_LOG(LogTemp, Log, TEXT("DarkWing Attack Omen"));
}

//공격
void ADarkWing::Attack(AActor* Target)
{
	// 플레이어를 향해 깃털 n개 발사
	if (nullptr != FeatherClass) {

		UE_LOG(LogTemp, Log, TEXT("Target :: %s"), *Target->GetName());
		FVector TargetLocation = Target->GetActorLocation();
		TargetLocation.Z = 0;

		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			for (uint32 i = 0; i < FeatherNum; i++) {
				FVector MuzzleLocation = AttackOffsets[i] + this->GetActorLocation();
				FRotator MuzzleRotation;

				ADW_FeatherProjectile* Feather = World->SpawnActor<ADW_FeatherProjectile>(FeatherClass, MuzzleLocation, MuzzleRotation, SpawnParams);
				Feather->SetVariables(FeatherPower, FeatherSpeed, FeatherDeleteTime);

				if (Feather) {
					FVector LaunchDirection = (TargetLocation - MuzzleLocation).GetSafeNormal();

					Feather->FireInDirection(LaunchDirection);
				}
			}
		}
	}
}
