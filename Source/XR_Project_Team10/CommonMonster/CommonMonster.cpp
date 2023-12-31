﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/CommonMonster/CommonMonster.h"
#include "XR_Project_Team10/AI/Common/KWCommonAIController.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"

ACommonMonster::ACommonMonster()
{
	PrimaryActorTick.bCanEverTick = true;

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

float ACommonMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Log, TEXT("TakeDamage"));
	MonsterCurrentHP -= DamageAmount;
	if (MonsterCurrentHP <= 0 && GetController()) {
		GetController()->Destroy();
		CommonMonsterDead();
	}
	AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(DamageCauser);
	if (PlayerCharacter) {
		KnockBackImpactLocation = PlayerCharacter->GetTruePlayerLocation()->GetActorLocation();
		KnockBackElapsedTime = 0;
		GetWorldTimerManager().SetTimerForNextTick(this, &ACommonMonster::ApplyKnockBack);
	}
	return 0;
}

void ACommonMonster::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->SetCollisionObjectType(ECC_ENEMY);
	GetCapsuleComponent()->SetCollisionProfileName(CP_ENEMY);
	GetMesh()->SetCollisionObjectType(ECC_ENEMY);
	GetMesh()->SetCollisionProfileName(CP_ENEMY);	
}

void ACommonMonster::SetCommonAttackDelegate(const FCommonAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ACommonMonster::AttackOmen()
{
	MonsterState = EState::E_ATTACK_OMEN;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ACommonMonster::Attack, 0.01f, true);
}

void ACommonMonster::Attack()
{
	if (MonsterState == EState::E_ATTACK) {
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
		FPPTimerHelper::InvalidateTimerHandle(AttackTimerHandle);
		GetWorldTimerManager().SetTimer(AttackCoolDownTimerHandle, this, &ACommonMonster::CheckAttackDelay, 0.01f, true);
		GetWorldTimerManager().SetTimer(AttackEndTimerHandle, this, &ACommonMonster::AttackEnd, 0.01f, true);

		AttackBehaviour();
	}
}

void ACommonMonster::AttackBehaviour()
{

}

void ACommonMonster::AttackEnd()
{
	if (MonsterState == EState::E_ATTACK_END) {
		OnAttackFinished.ExecuteIfBound();
		MonsterState = EState::E_IDLE;
		GetWorldTimerManager().ClearTimer(AttackEndTimerHandle);
		FPPTimerHelper::InvalidateTimerHandle(AttackEndTimerHandle);
		PlayerTarget = nullptr;
	}
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

	PlayerTarget = Target;

	MonsterState = EState::E_ATTACK_OMEN;
	GetWorldTimerManager().SetTimer(AttackOmenTimerHandle, this, &ACommonMonster::AttackOmen, MonsterAttackOmenTime, false);
}

void ACommonMonster::CommonMonsterDead()
{
	MonsterState = EState::E_DEAD;
	PlayDeadAnimation();
	if(Controller)
	{
		Controller->UnPossess();
	}
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	GetWorldTimerManager().SetTimer(AfterDeadTimerHandle, this, &ACommonMonster::AfterDead, MonsterDisableTime, false);
}

void ACommonMonster::PlayDeadAnimation()
{;
	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	GetMesh()->GetAnimInstance()->Montage_Play(DeadMontage, 1.0f);
}

void ACommonMonster::AfterDead()
{
	Destroy();
}

void ACommonMonster::PlayHitAnimation()
{
	if(MonsterCurrentHP > 0)
	{
		GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
		GetMesh()->GetAnimInstance()->Montage_Play(HitMontage, 1.5f);
	}
}

void ACommonMonster::ApplyKnockBack()
{
	if (MonsterState == EState::E_IDLE) {
		MonsterState = EState::E_HIT;
		PlayHitAnimation();
	}
	KnockBackElapsedTime += GetWorld()->DeltaTimeSeconds;
	if (KnockBackElapsedTime >= 0.5f)
	{
		MonsterState = EState::E_IDLE;
		return;
	}
	float OriginZ = GetActorLocation().Z;
	FVector KnockBackLocation = GetActorLocation() + (GetActorLocation() - KnockBackImpactLocation).GetSafeNormal() * KnockBackAmount;
	KnockBackLocation.Z = OriginZ;
	SetActorLocation(KnockBackLocation);
	GetWorldTimerManager().SetTimerForNextTick(this, &ACommonMonster::ApplyKnockBack);
}

