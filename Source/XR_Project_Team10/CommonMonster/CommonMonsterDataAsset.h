// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CommonMonsterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UCommonMonsterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "몬스터 체력")
	int32 MonsterHP;

	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "몬스터 이동속도")
	int32 MonsterMoveSpeed;


public:
	UPROPERTY(EditAnywhere, Category = Range, DisplayName = "몬스터 탐색 범위 반지름 값")
	float MonsterResearchRange;

	UPROPERTY(EditAnywhere, Category = Range, DisplayName = "몬스터 인지 범위 반지름 값")
	float MonsterRecognitionRange;

	UPROPERTY(EditAnywhere, Category = Range, DisplayName = "몬스터 추적 범위 반지름 값")
	float MonsterTrackingRange;

	UPROPERTY(EditAnywhere, Category = Range, DisplayName = "몬스터 공격 범위 확인 반지름 값")
	float MonsterAttackConfigRange;

public:

	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "몬스터 대기 상태 지속시간")
	float MonsterIdleTime;

	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "몬스터 탐색 상태 지속시간")
	float MonsterResearchTime;

	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "몬스터 인지 상태 지속시간")
	float MonsterRecognitionTime;

	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "몬스터 추적 상태 지속시간")
	float MonsterTrackingTime;
};
