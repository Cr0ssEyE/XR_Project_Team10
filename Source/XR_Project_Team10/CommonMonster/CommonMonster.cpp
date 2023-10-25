// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/CommonMonster/CommonMonster.h"
#include "XR_Project_Team10/AI/Common/KWCommonAIController.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"
#include "Components/CapsuleComponent.h"

ACommonMonster::ACommonMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	MonsterComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capshule"));
	MonsterComponent->SetSimulatePhysics(true);
	RootComponent = MonsterComponent;

	AIControllerClass = AKWCommonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (nullptr != MonsterData) {
		MonsterCurrentHP = MonsterData->MonsterHP;
		MonsterCurrentMoveSpeed = MonsterData->MonsterMoveSpeed;
	}
}

void ACommonMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACommonMonster::BeginPlay()
{
	Super::BeginPlay();
}

void ACommonMonster::SetCommonAttackDelegate(const FCommonAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ACommonMonster::AttackOmen(AActor* Target)
{
}

void ACommonMonster::Attack(AActor* Target)
{
}

void ACommonMonster::CheckAttackDelay()
{
	if (FPPTimerHelper::IsDelayElapsed(AttackCoolDownTimerHandle, MonsterAttackCoolDownTime)) {
		GetWorldTimerManager().ClearTimer(AttackCoolDownTimerHandle);
		FPPTimerHelper::InvalidateTimerHandle(AttackCoolDownTimerHandle);
	}
}

void ACommonMonster::CommonMonsterAttack(AActor* Target)
{
	if (GetWorldTimerManager().IsTimerActive(AttackCoolDownTimerHandle)) {
		OnAttackFinished.ExecuteIfBound();
		return;
	}

	AttackOmen(Target);
	Attack(Target);

	GetWorldTimerManager().SetTimer(AttackCoolDownTimerHandle, this, &ACommonMonster::CheckAttackDelay, 0.01f, true);
}

void ACommonMonster::CommonMonsterDead()
{
}

