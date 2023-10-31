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

	UPROPERTY(EditAnywhere, Category = Status, DisplayName = "호호누 히트박스 크기")
	FVector HohonuHitBoxCollision;

	UPROPERTY(EditAnywhere, Category = Status, DisplayName = "호호누 충돌시 넉백 배율")
	float HitKnockBackMultiplyValue;

	UPROPERTY(EditAnywhere, Category = Status, DisplayName = "호호누 충돌시 넉백 높이(배율 계산 후 적용)")
	float HitKnockBackHeightValue;
	
	UPROPERTY(EditAnywhere, Category = Status, DisplayName = "호호누 광폭화 체력")
	float HohonuLunaticHp;
	
	UPROPERTY(EditAnywhere, Category = Status, DisplayName = "호호누 기본 이동속도")
	float HohonuMoveSpeed;

	UPROPERTY(EditAnywhere, Category = Status, DisplayName = "디버그 뷰 활성화")
	uint8 bIsDebugEnable : 1;
	
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
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "수정 메시")
	TObjectPtr<UStaticMesh> SC_Mesh;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "수정 소환 이펙트")
	TObjectPtr<UNiagaraSystem> SC_SummonVFX;

	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "수정 낙하 이펙트")
	TObjectPtr<UNiagaraSystem> SC_DropDownVFX;

	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "수정 파동 이펙트")
	TObjectPtr<UNiagaraSystem> SC_WaveVFX;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "수정 파괴 이펙트")
	TObjectPtr<UNiagaraSystem> SC_DestroyVFX;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 생성 간격(초)")
	float SC_SpawnDelay;

	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 생성 높이")
	float SC_SpawnHeight;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 수정 체력")
	float SC_Hp;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 낙하 대미지")
	float SC_DropDownDamage;

	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 낙하 속도(초)")
	float SC_DropDownSpeed;

	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 낙하 지연 시간(초)")
	float SC_DropDownDelay;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 공격 파동 너비")
	float SC_WaveLength;

	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 공격 파동 대미지")
	float SC_WaveDamage;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 수정 공격 간격(초)")
	float SC_AttackDelay;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 공격 최대 범위(반지름)")
	float SC_MaxAttackRange;
	
	UPROPERTY(EditAnywhere, Category = Pattern_A, DisplayName = "호호누 수정 소환 공격 범위 증가 속도(초)")
	float SC_IncreaseAttackRange;

public:
	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 이펙트")
	TObjectPtr<UNiagaraSystem> SL_LaserVFX;

	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 좌우 무작위 시작(기본 값 왼쪽에서 오른쪽)")
	uint8 SL_bIsRandomStart : 1;
	
	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 데미지")
	float SL_Damage;

	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 부채꼴 각도")
	float SL_Degree;

	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 지속 시간(초)")
	float SL_ActiveTime;

	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 길이")
	float SL_Distance;
	
	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 판정 범위")
	FVector SL_DamageRange;

	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 넉백 배율")
	float SL_KnockBackMultiplyValue;

	UPROPERTY(EditAnywhere, Category = Pattern_B, DisplayName = "호호누 레이저 발사 넉백 높이(배율 계산 후 적용)")
	float SL_KnockBackHeightValue;
	
public:
	UPROPERTY(EditAnywhere, Category = Pattern_C, DisplayName = "호호누 양손 휘두르기 데미지")
	float MA_Damage;

	UPROPERTY(EditAnywhere, Category = Pattern_C, DisplayName = "호호누 양손 휘두르기 손 판정 범위")
	FVector MA_DamageRange;

	UPROPERTY(EditAnywhere, Category = Pattern_C, DisplayName = "호호누 양손 휘두르기 폭발 판정 범위")
	FVector MA_ExplodeDamageRange;

	UPROPERTY(EditAnywhere, Category = Pattern_C, DisplayName = "호호누 양손 휘두르기 넉백 배율")
	float MA_KnockBackMultiplyValue;

	UPROPERTY(EditAnywhere, Category = Pattern_C, DisplayName = "호호누 양손 휘두르기 넉백 높이(배율 계산 후 적용)")
	float MA_KnockBackHeightValue;
	
public:
	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 데미지")
	float WW_Damage;

	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 판정 범위")
	FVector WW_DamageRange;
	
	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 이동 속도 증가치(초)")
	float WW_IncreaseMoveSpeed;
	
	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 최대 이동 속도(초)")
	float WW_MaxMoveSpeed;

	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 회전 속도(초)")
	float WW_RotateSpeed;

	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 넉백 배율")
	float WW_KnockBackMultiplyValue;

	UPROPERTY(EditAnywhere, Category = Pattern_D, DisplayName = "호호누 훨윈드 넉백 높이(배율 계산 후 적용)")
	float WW_KnockBackHeightValue;
	
public:
	UPROPERTY(EditAnywhere, Category = Pattern_E, DisplayName = "호호누 백스텝 지속 시간(초)")
	float BS_Time;

	UPROPERTY(EditAnywhere, Category = Pattern_E, DisplayName = "호호누 백스텝 이동 속도(초)")
	float BS_MoveSpeed;

public:
	UPROPERTY(EditAnywhere, Category = Pattern_F, DisplayName = "호호누 연발레이저 이펙트")
	TObjectPtr<UNiagaraSystem> ML_LaserVFX;
	
	UPROPERTY(EditAnywhere, Category = Pattern_F, DisplayName = "호호누 연발 레이저 데미지")
	float ML_Damage;

	UPROPERTY(EditAnywhere, Category = Pattern_F, DisplayName = "호호누 연발 레이저 지속 시간(초)")
	float ML_AttackTime;
	
	UPROPERTY(EditAnywhere, Category = Pattern_F, DisplayName = "호호누 연발 레이저 추적 속도(초)")
	float ML_TurnSpeed;
	
};
