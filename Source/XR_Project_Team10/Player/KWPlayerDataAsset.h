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
	UPROPERTY(EditAnywhere, Category = Mesh, DisplayName = "PlayerMesh(Static)")
	TObjectPtr<class UStaticMesh> PlayerMesh;
	
	UPROPERTY(EditAnywhere, Category = InputValue)
	float AddMoveForceValue;

	UPROPERTY(EditAnywhere, Category = InputValue)
	float AddJumpForceValue;

	UPROPERTY(EditAnywhere, Category = InputValue)
	float MaxMoveForceValue;

	UPROPERTY(EditAnywhere, Category = InputValue)
	float JustTimingValue;
	
	UPROPERTY(EditAnywhere, Category = InputValue)
	float JumpDelayTime;

	UPROPERTY(EditAnywhere, Category = InputValue)
	float BoostAddForceValue;

	UPROPERTY(EditAnywhere, Category = InputValue)
	float BoostAddMaxForceValue;
	
	UPROPERTY(EditAnywhere, Category = InputValue)
	float BoostDurationTime;

	UPROPERTY(EditAnywhere, Category = InputValue)
	float BoostCoolDownTime;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> PlayerInputMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ToggleTypeAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> BoostAction;
};
