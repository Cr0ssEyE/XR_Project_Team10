// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/CommonMonster/CommonMonsterAnimInstance.h"

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
}
