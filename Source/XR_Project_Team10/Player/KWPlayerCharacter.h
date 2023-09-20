// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "KWPlayerCharacter.generated.h"

UENUM()
enum class EGearState : uint8 
{
	GearOne = 0,
	GearTwo = 1,
	GearThree = 2,
	GearFour = 3,
	EndOfGearState = 4
};

// 플레이어 캐릭터
UCLASS()
class XR_PROJECT_TEAM10_API AKWPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/*
	* 약어 정리
	* DA = DashAttack
	* RB = ReBound
	* RBD = ReBoundDash
	*/
public:
	// Sets default values for this character's properties
	AKWPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Default Data
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UKWPlayerDataAsset> CharacterData;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> PlayerComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> RollingMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 *	유저 입력 관련 함수 리스트
	 **/
protected:
	void MoveAction(const FInputActionValue& Value);
	void MoveActionCompleted(const FInputActionValue& Value);
	void JumpAddForceAction(const FInputActionValue& Value);

	void ToggleCharacterTypeAction(const FInputActionValue& Value);
	void RB_JustTimingAction(const FInputActionValue& Value);
	void AttackActionSequence(const FInputActionValue& Value);
	void AttackCoolDownTimer();
	void VelocityDecelerateTimer();
	
	/**
	 * 유저 입력 관련 변수 리스트
	 **/
private:
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<class UInputAction> ToggleTypeAction;

	UPROPERTY()
	TObjectPtr<class UInputAction> MoveInputAction;

	UPROPERTY()
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY()
	TObjectPtr<class UInputAction> AttackAction;

	/**
	 * 캐릭터 움직임 관련 변수 리스트
	 **/
private:
	UPROPERTY()
	FVector2D MoveInputValue;
	
	UPROPERTY()
	float DefaultVelocityValue;

	UPROPERTY()
	float DefaultMaxVelocityValue;
	
	UPROPERTY()
	TArray<float> VelocityMultiplyValuesByGear;
	
	UPROPERTY()
	TArray<float> MaxVelocityMagnificationByGear;

	UPROPERTY()
	float AddJumpForceValue;
	
	FTimerHandle JumpDelayTimerHandle;
	
	FTimerHandle DropDownTimerHandle;
	
	FTimerHandle JustTimingTimerHandle;
	
	FTimerHandle VelocityDecelerationTimerHandle;
	
	UPROPERTY()
	float JustTimingTime;
	
	UPROPERTY()
	float JustTimingElapsedTime;
	
	UPROPERTY()
	float JumpDelayTime;
	
	UPROPERTY()
	float VelocityDecelerateTime;

	UPROPERTY()
	FVector VelocityDecelerateTarget;

	/** 공격 관련 변수 리스트 \n
	 * AttackDash를 AD_ 와 같은 형태로 축약해서 표현한다.
	**/
private:
	FTimerHandle DA_DurationTimerHandle;
	
	FTimerHandle AttackCoolDownTimerHandle;
	
	UPROPERTY()
	float DA_AddVelocityValue;
	
	UPROPERTY()
	float DA_DurationTime;

	UPROPERTY()
	float AttackCoolDownTime;

	UPROPERTY()
	float DropDownVelocityValue;

	UPROPERTY()
	float DropDownMinimumHeightValue;
	
	/** 리바운드 관련 변수 리스트 \n
	 * ReBound를 RB_와 같은 형태로 축약해서 표현한다. \n
	 * ReBoundDash를 RBD_와 같은 형태로 축약해서 표현한다.
	**/
private:
	UPROPERTY()
	float RB_AddForceValue;
	
	UPROPERTY()
	float RBD_AddVelocityValue;


	/**
	 *	플레이어 상태 관련 함수 리스트
	 **/
private:
	void CheckGearState();
	
	/**
	 * 플레이어 상태 관련 변수 리스트
	 **/
private:
	FTimerHandle CheckGearStateTimerHandle;
	
	EGearState CurrentGearState;

	TArray<FVector> ColorsByGear;
	
	uint32 bIsDead : 1;
	
	uint32 bIsMoving : 1;
	
	uint32 bIsRolling : 1;

	uint32 bIsJumping : 1;
	
	uint32 bIsAttackOnGoing : 1;
};
