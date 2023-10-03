// Fill out your copyright notice in the Description page of Project Settings.

#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"
#include "XR_Project_Team10/CommonMonster/DarkWing.h"
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
}

void ADarkWing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttackConfig();
}

void ADarkWing::BeginPlay()
{
	Super::BeginPlay();

	FVector MuzzleLocation = AttackOffsets[0] + GetActorLocation();
	FRotator MuzzleRotation;

	UWorld* World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		ADW_FeatherProjectile* Feather = World->SpawnActor<ADW_FeatherProjectile>(FeatherClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Feather) {
			FVector LaunchDirection = MuzzleRotation.Vector();
			Feather->FireInDirection(FVector(0, 10, 0));
		}
	}
}

void ADarkWing::Research()
{
}

void ADarkWing::Recognition()
{
}

void ADarkWing::Tracking()
{
}

//공격 확인
void ADarkWing::AttackConfig()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Player), false, this);
	//FCollisionQueryParams Params(NAME_None, false, this);

	float AttackCheckRange = MonsterData->MonsterAttackConfigRange;

	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * AttackCheckRange;

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackCheckRange),
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackCheckRange;
	FVector Center = GetActorLocation();
	float HalfHeight = AttackCheckRange * 0.5f + AttackCheckRange;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1;

	DrawDebugCapsule
	(
		GetWorld(),
		Center,
		HalfHeight,
		AttackCheckRange,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);
#endif

	if (bResult) {
		UE_LOG(LogTemp, Log, TEXT("%s"), *HitResult.GetActor()->GetName());
	}
}

//공격 전조
void ADarkWing::AttackOmen()
{
	// 전조
}

//공격
void ADarkWing::Attack()
{
	// 플레이어를 향해 깃털 n개 발사
	if (nullptr != FeatherClass) {
		//FVector MuzzleLocation = 
	}
}

void ADarkWing::Dead()
{
}
