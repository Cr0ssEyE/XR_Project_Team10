// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Engine/DataAsset.h"
#include "KWBossHohonuDataAsset.generated.h"

/**
 * 보스 몬스터에게 필요한 정보를 저장하는 데이터 에셋 \n
 * 호호누(Hohonu)를 기준으로 작성함.
 **/
UCLASS()
class XR_PROJECT_TEAM10_API UKWBossHohonuDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Mesh, DisplayName = "호호누 본체 메시")
	TObjectPtr<USkeletalMesh> HohonuMesh;
	
	UPROPERTY(EditAnywhere, Category = Mesh, DisplayName = "호호누 링 이펙트")
	TObjectPtr<UNiagaraSystem> HohonuRingEffect;

	UPROPERTY(EditAnywhere, Category = Mesh, DisplayName = "호호누 머리 이펙트")
	TObjectPtr<UNiagaraSystem> HohonuHeadEffect;

public:
	UPROPERTY(EditAnywhere, Category = Status, DisplayName = "호호누 체력")
	float HohonuHp;

	// 사용 여부 아직 불투명해서 주석 처리
	// UPROPERTY(EditAnywhere, Category = Status, DisplayName = "호호누 광폭화 체력")
	// float HohonuLunaticHp;
	
	UPROPERTY(EditAnywhere, Category = Status, DisplayName = "호호누 기본 이동속도")
	float HohonuMoveSpeed;

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
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 수정 갯수")
	float SC_Count;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 수정 체력")
	float SC_Hp;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 데미지")
	float SC_Damage;

	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 낙하 속도")
	float SC_Speed;

	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 수정 공격 간격(초)")
	float SC_AttackDelay;

	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 공격 최대 범위")
	float SC_MaxAttackRange;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 공격 범위 증가 속도")
	float SC_IncreaseAttackRange;

public:
	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 데미지")
	float SL_Damage;

	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 전조 시간(초)")
	float SL_AttackDelay;
	
	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 부채꼴 각도")
	float SL_Degree;

	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 회전 속도")
	float SL_TurnSpeed;
	
	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 범위")
	float SL_AttackRange;

public:
	UPROPERTY(EditAnywhere, Category = Pattern_C, DisplayName = "호호누 양손 휘두르기 데미지")
	float MA_Damage;

	UPROPERTY(EditAnywhere, Category = Pattern_C, DisplayName = "호호누 양손 휘두르기 속도")
	float MA_AttackSpeed;
	
public:
	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 데미지")
	float WW_Damage;

	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 전조 시간(초)")
	float WW_AttackDelay;

	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 지속 시간(초)")
	float WW_AttackTime;
	
	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 초당 이동 속도 증가치")
	float WW_IncreaseMoveSpeed;
	
	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 최대 이동 속도")
	float WW_MaxMoveSpeed;

public:
	UPROPERTY(EditAnywhere, Category = Pattern_E, DisplayName = "호호누 백스텝 거리")
	float BS_Range;

	UPROPERTY(EditAnywhere, Category = Pattern_E, DisplayName = "호호누 백스텝 이동 속도")
	float BS_MoveSpeed;

public:
	UPROPERTY(EditAnywhere, Category = Pattern_F, DisplayName = "호호누 연발 레이저 데미지")
	float ML_Damage;

	UPROPERTY(EditAnywhere, Category = Pattern_F, DisplayName = "호호누 연발 레이저 지속 시간")
	float ML_AttackTime;
	
	UPROPERTY(EditAnywhere, Category = Pattern_F, DisplayName = "호호누 연발 레이저 추적 속도")
	float ML_TurnSpeed;
	
};
