// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/CommonMonster/CommonMonsterAnimInstance.h"
#include "XR_Project_Team10/CommonMonster/CommonMonster.h"

UCommonMonsterAnimInstance::UCommonMonsterAnimInstance()
{
}

void UCommonMonsterAnimInstance::NativeInitializeAnimation()
{
	CurrentPawnSpeed = 0.0;
}

void UCommonMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
	ACommonMonster* Monster = Cast<ACommonMonster>(Pawn);
	if (Monster) {
		if (Monster->GetMonsterState() == EState::E_ATTACK_OMEN) {
			IsAttack = true;
		}
		else if (Monster->GetMonsterState() == EState::E_ATTACK) {
			IsAttack = true;
		}
		else {
			IsAttack = false;
		}
	}
}
