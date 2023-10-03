// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "XR_Project_Team10/Enumeration/KWHohonuPattern.h"
#include "KWBossHohonuAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOmenPatternDelegate, EHohonuPattern)
DECLARE_MULTICAST_DELEGATE_TwoParams(FPatternToggleDelegate, EHohonuPattern, bool)

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWBossHohonuAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UKWBossHohonuAnimInstance();

	FOmenPatternDelegate OmenPatternDelegate;
	FPatternToggleDelegate PatternToggleDelegate;
	
protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_OmenSummonCrystal() { OmenPatternDelegate.Broadcast(EHohonuPattern::SummonCrystal); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_OmenSweepLaser() { OmenPatternDelegate.Broadcast(EHohonuPattern::SweepLaser); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_OmenWhirlWind() { OmenPatternDelegate.Broadcast(EHohonuPattern::WhirlWind); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_SpawnCrystal() { PatternToggleDelegate.Broadcast(EHohonuPattern::SummonCrystal, true); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_FireLaser() { PatternToggleDelegate.Broadcast(EHohonuPattern::SweepLaser, true); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_BackStepBegin() { PatternToggleDelegate.Broadcast(EHohonuPattern::BackStep, true); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_WhirlWindHitCheckBegin() { PatternToggleDelegate.Broadcast(EHohonuPattern::WhirlWind, true); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_WhirlWindHitCheckEnd() { PatternToggleDelegate.Broadcast(EHohonuPattern::WhirlWind, false); }
	
};
