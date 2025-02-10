// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "KWPlayerDataAsset.generated.h"

/**
 * 플레이어 캐릭터 관련 데이터 에셋
 */

/*
	* 약어 정리
	* DA = DashAttack
	* RB = ReBound
	* RBD = ReBoundDash
*/
UCLASS()
class XR_PROJECT_TEAM10_API UKWPlayerDataAsset : public UDataAsset
{
	GENERATED_BODY()

	// 캐릭터 관련 변수
public:
	UPROPERTY(EditAnywhere, Category = Default, DisplayName = "플레이어 체력")
	int PlayerHp;
	
	UPROPERTY(EditAnywhere, Category = Default, DisplayName = "플레이어 구르기 상태 메시")
	TObjectPtr<class USkeletalMesh> PlayerRollingMesh;

	UPROPERTY(EditAnywhere, Category = Default, DisplayName = "플레이어 걷기 상태 메시")
	TObjectPtr<class USkeletalMesh> PlayerWalkingMesh;

	UPROPERTY(EditAnywhere, Category = Default, DisplayName = "플레이어 죽음 애니메이션 몽타주")
	TObjectPtr<UAnimMontage> KiwiAnimMontage;
	
public:
	UPROPERTY(EditAnywhere, Category = Debug, DisplayName = "히트 체크 디버그 뷰 활성화")
	uint8 bIsEnableHitCheckDebugView : 1;

	UPROPERTY(EditAnywhere, Category = Debug, DisplayName = "기어 디버그 뷰 활성화")
	uint8 bIsEnableGearDebugView : 1;

	UPROPERTY(EditAnywhere, Category = Debug, DisplayName = "속도 디버그 뷰 활성화")
	uint8 bIsEnableVelocityDebugView : 1;

	UPROPERTY(EditAnywhere, Category = Debug, DisplayName = "좌표 디버그 뷰 활성화")
	uint8 bIsEnableLocationDebugView : 1;
	
	// 카메라 관련 변수
public:
	UPROPERTY(EditAnywhere, Category = View, DisplayName = "카메라 FOV")
	float CameraFOV;
	
	UPROPERTY(EditAnywhere, Category = View, DisplayName = "스프링 암 길이")
	float SpringArmLength;

	UPROPERTY(EditAnywhere, Category = View, DisplayName = "스프링 암 기울기")
    float SpringArmAngle;

	// 기본 상태 관련 변수
public:
	UPROPERTY(EditAnywhere, Category = Walking, DisplayName = "걷기 이펙트")
	TObjectPtr<UNiagaraSystem> WalkingNiagaraSystem;
	
	UPROPERTY(EditAnywhere, Category = Walking, DisplayName = "구르기 이펙트")
	TObjectPtr<UNiagaraSystem> RollingNiagaraSystem;
	
	UPROPERTY(EditAnywhere, Category = Walking, DisplayName = "점프 이펙트")
	TObjectPtr<UNiagaraSystem> JumpingNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = Walking, DisplayName = "피격 이펙트")
	TObjectPtr<UNiagaraSystem> HitNiagaraSystem;
	
	UPROPERTY(EditAnywhere, Category = Walking, DisplayName = "걷기 상태 이동 속도")
	float WakingStateMoveSpeed;
	
	UPROPERTY(EditAnywhere, Category = Walking, DisplayName = "걷기 상태 점프 높이")
	float WakingStateJumpValue;

	UPROPERTY(EditAnywhere, Category = Walking, DisplayName = "걷기 상태 공중 이동 입력 민감도(0 ~ 1)")
	float WakingStateAirControl;
	
	UPROPERTY(EditAnywhere, Category = Walking, DisplayName = "걷기 상태 중력 영향 배율")
	float WakingStateGravityScale;

	// 구르기 상태 관련 변수
public:
	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "이동할 경우 구르기 상태 강제 변환")
	uint8 bIsMovingMustRolling : 1;

	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "구르기 상태에서 일정 속도 이하에서 이동 입력 없을 경우 걷기로 전환")
	uint8 bIsRollingIdleToWalk : 1;
	
	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "구르기 상태 기본 이동속도 증가 값")
	float DefaultVelocityValue;

	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "구르기 상태 기본 이동속도 최대 값")
	float DefaultMaxVelocityValue;

	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "구르기 상태 상호작용 관련 이동속도 최대 값")
	float SystemMaxVelocityValue;
	
	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "기어 전환 이동속도 배율 값")
	TArray<float> MaxVelocityMagnificationByGear;

	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "테스트용 기어 별 색상 값(최대 4개)")
	TArray<FLinearColor> ColorsByGear;

	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "구르기 상태 점프 높이")
	float AddJumpForceValue;
	
	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "점프 딜레이")
	float JumpDelayTime;

	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "감속 시간")
	float DecelerateTime = 1.f;

	UPROPERTY(EditAnywhere, Category = Rolling, DisplayName = "감속 그래프 유형")
	TEnumAsByte<EEasingFunc::Type> DecelerateEasingFunction;
	
	// 공격 관련 변수
public:
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "대시 이펙트")
	TObjectPtr<UNiagaraSystem> DA_MoveNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "대시 타격 이펙트")
	TObjectPtr<UNiagaraSystem> DA_AttackNiagaraSystem;
	
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "공중 대시 사용 여부")
	uint8 bCanDashOnFlying : 1;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "대시 공격 기본 데미지")
	float DA_BaseDamage;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "대시 공격 기어 별 배율(최대 4개)")
	TArray<float> DA_MultiplyDamageByGear;
	
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "대시 공격 이동속도")
	float DA_AddVelocityValue;
	
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "대시 공격 지속시간")
	float DA_DurationTime;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "대시 공격 후 감속 배율")
	float DA_DecelerateValue;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "대시 공격 대기 시간")
	float AttackCoolDownTime;
	
public:
	UPROPERTY(EditAnywhere, Category = FileDriver, DisplayName = "내려찍기 공격 데미지")
	float DropDownDamage;
	
	UPROPERTY(EditAnywhere, Category = FileDriver, DisplayName = "내려찍기 공격 속도 값")
	float DropDownVelocityValue;

	UPROPERTY(EditAnywhere, Category = FileDriver, DisplayName = "내려찍기 공격 최소 높이 값")
	float DropDownMinimumHeightValue;
	
	UPROPERTY(EditAnywhere, Category = FileDriver, DisplayName = "내려찍기 공격 재사용 대기 시간")
	float DropDownCoolDownTime;

	// 리바운드 관련 변수
public:
	UPROPERTY(EditAnywhere, Category = ReBound, DisplayName = "충돌체 별 리바운드 배수")
	TArray<float> RB_MultiplyValuesByObjectType;

	UPROPERTY(EditAnywhere, Category = ReBound, DisplayName = "기어 별 리바운드 배수")
	TArray<float> RB_MultiplyValuesByGear;

	UPROPERTY(EditAnywhere, Category = ReBound, DisplayName = "리바운드 대시 입력 시간")
	float RBD_JustTimingCheckTime;

	UPROPERTY(EditAnywhere, Category = ReBound, DisplayName = "리바운드 대시 입력 실패시 이동 비활성화 시간")
	float RB_DisableMovementTime;
	
	UPROPERTY(EditAnywhere, Category = ReBound, DisplayName = "리바운드 대시 이동속도")
	float RBD_AddVelocityValue;

	// 입력 관련 변수
public:
	UPROPERTY(EditAnywhere, Category = InputContext, DisplayName = "키 입력 매핑 IMC")
	TObjectPtr<class UInputMappingContext> PlayerInputMappingContext;

	UPROPERTY(EditAnywhere, Category = InputContext, DisplayName = "플레이어 모드 전환 입력 키")
	TObjectPtr<class UInputAction> ToggleTypeAction;

	UPROPERTY(EditAnywhere, Category = InputContext, DisplayName = "플레이어 이동 입력 키")
	TObjectPtr<class UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, Category = InputContext, DisplayName = "점프 입력 키")
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = InputContext, DisplayName = "플레이어 대시 및 리바운드 입력 키")
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = InputContext, DisplayName = "플레이어 파일 드라이버 입력 키")
	TObjectPtr<class UInputAction> DropDownAction;

	UPROPERTY(EditAnywhere, Category = InputContext, DisplayName = "플레이어 일시정지 입력 키")
	TObjectPtr<class UInputAction> PauseGameAction;
	
};
