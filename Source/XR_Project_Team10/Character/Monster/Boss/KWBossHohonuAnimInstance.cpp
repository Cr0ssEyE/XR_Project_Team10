// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Character/Monster/Boss/KWBossHohonuAnimInstance.h"

UKWBossHohonuAnimInstance::UKWBossHohonuAnimInstance()
{
	
}

void UKWBossHohonuAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<AKWBossMonsterHohonu>(GetOwningActor());
	if(OwnerCharacter)
	{
		// PatternActivateDelegate.AddUObject(OwnerCharacter, &AKWBossMonsterHohonu::ActivatePatternExecute);
	}
}

void UKWBossHohonuAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UKWBossHohonuAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
