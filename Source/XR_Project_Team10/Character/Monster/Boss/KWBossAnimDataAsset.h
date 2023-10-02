// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KWBossAnimDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWBossAnimDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Animation, DisplayName = "보스 몬스터 ABP")
	TObjectPtr<UAnimBlueprint> BossAnimBlueprint;
	
	UPROPERTY(EditAnywhere, Category = Animation, DisplayName = "보스 몬스터 조우 애니메이션 몽타주")
	TObjectPtr<UAnimMontage> BossEncounterAnimMontage;

	UPROPERTY(EditAnywhere, Category = Animation, DisplayName = "보스 몬스터 패턴 애니메이션 몽타주")
	TObjectPtr<UAnimMontage> BossPatternAnimMontage;

	UPROPERTY(EditAnywhere, Category = Animation, DisplayName = "보스 몬스터 사망 애니메이션 몽타주")
	TObjectPtr<UAnimMontage> BossDeadAnimMontage;
};
