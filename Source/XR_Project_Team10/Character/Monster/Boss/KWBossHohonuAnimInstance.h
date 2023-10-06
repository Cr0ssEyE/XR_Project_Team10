// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KWBossMonsterHohonu.h"
#include "Animation/AnimInstance.h"
#include "XR_Project_Team10/Enumeration/KWHohonuPattern.h"
#include "KWBossHohonuAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FEndEncounterAnimDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FOmenPatternDelegate, EHohonuPattern)
DECLARE_MULTICAST_DELEGATE_OneParam(FActivatePatternDelegate, EHohonuPattern)
DECLARE_MULTICAST_DELEGATE(FDeActivatePatternDelegate)
/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWBossHohonuAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UKWBossHohonuAnimInstance();

	FEndEncounterAnimDelegate EndEncounterAnimDelegate;
	// FOmenPatternDelegate OmenPatternDelegate;
	FActivatePatternDelegate PatternActivateDelegate;
	FDeActivatePatternDelegate PatternDeActivateDelegate;
protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY()
	TObjectPtr<AKWBossMonsterHohonu> OwnerCharacter;
	
private:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_EncounterEnd() { EndEncounterAnimDelegate.Broadcast(); }
	/*
private:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_OmenSummonCrystal() { OmenPatternDelegate.Broadcast(EHohonuPattern::SummonCrystal); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_OmenSweepLaser() { OmenPatternDelegate.Broadcast(EHohonuPattern::SweepLaser); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_OmenWhirlWind() { OmenPatternDelegate.Broadcast(EHohonuPattern::WhirlWind); }
	*/
private:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_SpawnCrystal() { PatternActivateDelegate.Broadcast(EHohonuPattern::SummonCrystal); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_FireLaser() { PatternActivateDelegate.Broadcast(EHohonuPattern::SweepLaser); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_BackStepBegin() { PatternActivateDelegate.Broadcast(EHohonuPattern::BackStep); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_WhirlWindHitCheckBegin() { PatternActivateDelegate.Broadcast(EHohonuPattern::WhirlWind); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_WhirlWindHitCheckEnd() { PatternDeActivateDelegate.Broadcast(); }
	
};
