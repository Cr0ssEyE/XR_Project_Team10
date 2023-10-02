// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KWBossHohonuAIDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWBossHohonuAIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = AI, DisplayName = "호호누 근접 체크 거리")
	float TargetNearbyDistance;

	UPROPERTY(EditAnywhere, Category = AI, DisplayName = "호호누 광폭화 체력")
	float LunaticHp;
	
	 /**
	 * 호호누 패턴 별 약자 \n
	 * 수정 소환 = Summon Crystal = SC_ \n
	 * 레이저 발사 = SweepLaser = SL_ \n
	 * 양손 휘두르기 = MeleeAttack = MA_ \n
	 * 훨윈드 = Whirlwind = WW_ \n
	 * 백스텝 = BackStep = BS_ \n
	 * 연발 레이저 = MultipleLaser = ML_
	 **/
public:
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 패턴 사용 후 딜레이(초)")
	float SC_TerminateDelay;

	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 패턴 재사용 대기시간(초)")
	float SC_CoolDown;

public:
	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 패턴 사용 후 딜레이(초)")
	float SL_TerminateDelay;

	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 패턴 재사용 대기시간(초)")
	float SL_CoolDown;

public:
	UPROPERTY(EditAnywhere, Category = Pattern_C, DisplayName = "호호누 양 손 휘두르기 패턴 사용 후 딜레이(초)")
	float MA_TerminateDelay;

	UPROPERTY(EditAnywhere, Category = Pattern_C, DisplayName = "호호누 양 손 휘두르기 패턴 재사용 대기시간(초)")
	float MA_CoolDown;
	
public:
	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 패턴 사용 후 딜레이(초)")
	float WW_TerminateDelay;

	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 패턴 재사용 대기시간(초)")
	float WW_CoolDown;
	
public:
	UPROPERTY(EditAnywhere, Category = Pattern_E, DisplayName = "호호누 백스텝 패턴 사용 후 딜레이(초)")
	float BS_TerminateDelay;

	UPROPERTY(EditAnywhere, Category = Pattern_E, DisplayName = "호호누 백스텝 패턴 재사용 대기시간(초)")
	float BS_CoolDown;
	
public:
	UPROPERTY(EditAnywhere, Category = Pattern_F, DisplayName = "호호누 연발 레이저 패턴 사용 후 딜레이(초)")
	float ML_TerminateDelay;

	UPROPERTY(EditAnywhere, Category = Pattern_F, DisplayName = "호호누 연발 레이저 패턴 재사용 대기시간(초)")
	float ML_CoolDown;
};
