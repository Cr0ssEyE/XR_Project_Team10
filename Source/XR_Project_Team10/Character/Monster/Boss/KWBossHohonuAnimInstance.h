// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KWBossHohonuAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWBossHohonuAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UKWBossHohonuAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
