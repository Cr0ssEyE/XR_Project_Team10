// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Engine/DataAsset.h"
#include "KWPlayerDataAsset.generated.h"

/**
 * 플레이어 캐릭터 관련 데이터 에셋
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWPlayerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Mesh, DisplayName = "플레이어 구르기 상태 메시")
	TObjectPtr<class UStaticMesh> PlayerRollingMesh;

	UPROPERTY(EditAnywhere, Category = Mesh, DisplayName = "플레이어 걷기 상태 메시")
	TObjectPtr<class USkeletalMesh> PlayerWalkingMesh;

public:
	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "구르기 상태 기본 이동속도 증가 값")
	float DefaultVelocityValue;

	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "구르기 상태 기본 이동속도 최대 값")
	float DefaultMaxVelocityValue;
	
	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "기어 별 구르기 상태 이동속도 증가 배율 값")
	TArray<float> VelocityMultiplyValuesByGear;

	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "기어 전환 이동속도 배율 값")
	TArray<float> MaxVelocityMagnificationByGear;

	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "테스트용 기어 별 색상 값")
	TArray<FVector> ColorsByGear;
	
	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "테스트용 점프 속도 값")
	float AddJumpForceValue;

	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "리바운드 대시 입력 시간")
	float JustTimingValue;
	
	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "테스트용 점프 대기 시간")
	float JumpDelayTime;

public:
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "대시 공격 이동속도")
	float DA_AddVelocityValue;
	
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "대시 공격 지속시간")
	float DA_DurationTime;
	
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "내려찍기 공격 속도 값")
	float DropDownVelocityValue;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "내려찍기 공격 최소 높이 값")
	float DropDownMinimumHeightValue;
	
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "공격 대기 시간")
	float AttackCoolDownTime;

public:
	UPROPERTY(EditAnywhere, Category = InputContext)
	TObjectPtr<class UInputMappingContext> PlayerInputMappingContext;

	UPROPERTY(EditAnywhere, Category = InputContext)
	TObjectPtr<class UInputAction> ToggleTypeAction;

	UPROPERTY(EditAnywhere, Category = InputContext)
	TObjectPtr<class UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, Category = InputContext)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = InputContext)
	TObjectPtr<class UInputAction> AttackAction;
};
