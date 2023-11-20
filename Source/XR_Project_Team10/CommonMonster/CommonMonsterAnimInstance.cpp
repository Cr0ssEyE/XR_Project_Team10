// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/CommonMonster/CommonMonsterAnimInstance.h"

UCommonMonsterAnimInstance::UCommonMonsterAnimInstance()
{
}

void UCommonMonsterAnimInstance::NativeInitializeAnimation()
{
	auto Pawn = TryGetPawnOwner();
	if (Pawn)
		Monster = Cast<ACommonMonster>(Pawn);
	CurrentPawnSpeed = 0.0;
}

void UCommonMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Monster) {
		CurrentPawnSpeed = Monster->GetVelocity().Size();

		if (Monster->GetMonsterState() == EState::E_DEAD) {
			IsDead = true;
		}
		else if (Monster->GetMonsterState() == EState::E_ATTACK_OMEN ||
			Monster->GetMonsterState() == EState::E_ATTACK) {
			IsAttack = true;
		}
		else {
			IsAttack = false;
		}
	}
}

//Anim notify 확인 이후 monster state 변경, Attack 및 Attack end 실행

void UCommonMonsterAnimInstance::AnimNotify_Attack()
{
	UE_LOG(LogTemp, Log, TEXT("Anim Attack Start"));
	Monster->SetMonsterState(EState::E_ATTACK);
}

void UCommonMonsterAnimInstance::AnimNotify_AttackEnd()
{
	UE_LOG(LogTemp, Log, TEXT("Anim Attack End"));
	Monster->SetMonsterState(EState::E_ATTACK_END);
}
