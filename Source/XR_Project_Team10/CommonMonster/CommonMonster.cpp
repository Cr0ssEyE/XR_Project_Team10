// Fill out your copyright notice in the Description page of Project Settings.


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


	//MonsterComponent = GetCapsuleComponent();

	// MonsterComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capshule"));
	// MonsterComponent->SetSimulatePhysics(true);
	//  RootComponent = MonsterComponent;

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
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ACommonMonster::Attack, MonsterAttackTime, false);
}

void ACommonMonster::Attack()
{
	MonsterState = EState::E_ATTACK;
	GetWorldTimerManager().SetTimer(AttackCoolDownTimerHandle, this, &ACommonMonster::CheckAttackDelay, 0.01f, true);
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
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void ACommonMonster::AfterDead()
{
	Destroy();
}

void ACommonMonster::ApplyKnockBack()
{
	KnockBackElapsedTime += GetWorld()->DeltaTimeSeconds;
	if (KnockBackElapsedTime >= 0.5f)
	{
		return;
	}
	float OriginZ = GetActorLocation().Z;
	FVector KnockBackLocation = GetActorLocation() + (GetActorLocation() - KnockBackImpactLocation).GetSafeNormal() * KnockBackAmount;
	KnockBackLocation.Z = OriginZ;
	SetActorLocation(KnockBackLocation);
	GetWorldTimerManager().SetTimerForNextTick(this, &ACommonMonster::ApplyKnockBack);
}

