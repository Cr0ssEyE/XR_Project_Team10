// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "KWPlayerWidgetController.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "XR_Project_Team10/Enumeration/KWPlayerGearType.h"
#include "XR_Project_Team10/Enumeration/KWObjectType.h"
#include "KWPlayerCharacter.generated.h"

// 플레이어 캐릭터
UCLASS(BlueprintType, Blueprintable)
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

	FORCEINLINE AActor* GetTruePlayerLocation() { return Cast<AActor>(PlayerTrueLocation); }
	FORCEINLINE float GetHp() { return PlayerHp; }
	FORCEINLINE void SetHp(float Value) { PlayerHp = Value; }

	FORCEINLINE bool CheckIsAccelerated() { return bIsAccelerated; }
	FORCEINLINE void SetAccelerate();

	FORCEINLINE bool CheckRolling() { return bIsRolling; }
	FORCEINLINE void ToggleTypeToRolling() { if(!bIsRolling) { ToggleCharacterType(); }; }
	
 	UFUNCTION(BlueprintCallable)
	FORCEINLINE UKWPlayerWidgetController* GetWidgetController() { return PlayerWidgetController; }

	UFUNCTION(BlueprintCallable)
	UAudioComponent* GetMainAudioComponent() { return KiwiMainAudioComponent; }
	
	UFUNCTION(BlueprintCallable)
	UAudioComponent* GetSubAudioComponent() { return KiwiSubAudioComponent; }
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> KiwiMainAudioComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> KiwiSubAudioComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Default Data
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> RootMesh;

	UPROPERTY()
	TObjectPtr<class UStaticMesh> RootStaticMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UKWPlayerDataAsset> CharacterData;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AKWLocationDetector> PlayerTrueLocation;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> PlayerComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USkeletalMesh> WalkingMesh;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UAnimInstance> WalkingAnimInstance;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USkeletalMeshComponent> RollingMeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USkeletalMesh> RollingMesh;
	
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UAnimInstance> RollingAnimInstance;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> KiwiAnimMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UKWPlayerWidgetController> PlayerWidgetController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> KiwiModeNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> RollingModeNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> AttackEffectNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> EventNiagaraComponent;
	
	UPROPERTY()
	float PlayerHp;

	UPROPERTY()
	FVector MeshOriginScale;
	
	uint8 bIsEnableHitCheckDebugView : 1;

	uint8 bIsEnableGearDebugView : 1;
	
	uint8 bIsEnableVelocityDebugView : 1;
	
	uint8 bIsEnableLocationDebugView : 1;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	FORCEINLINE TObjectPtr<class UStaticMeshComponent> GetMeshComp() { return RootMesh; }
	
	/**
	 *	유저 입력 관련 함수 리스트
	 **/
protected:
	void MoveAction(const FInputActionValue& Value);
	void MoveActionCompleted(const FInputActionValue& Value);
	void JumpAddForceAction(const FInputActionValue& Value);

	void ToggleCharacterTypeAction(const FInputActionValue& Value);
	void AttackActionSequence(const FInputActionValue& Value);
	void DropDownActionSequence(const FInputActionValue& Value);
	void TogglePauseWidget();
	
	/**
	 * 공격 입력 분기 함수 \n
	 * RBD = 리바운드 대시
	 * DA = 대시 어택
	 * FD = 파일 드라이버
	 **/
private:
	void ToggleCharacterType();
	void CheckIdleStateWhenRolling();
	void RBD_JustTimingProceedAction();
	void DA_ProceedAction();
	void FD_ProceedAction();
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

	UPROPERTY()
	TObjectPtr<class UInputAction> FileDriverAction;

	UPROPERTY()
	TObjectPtr<class UInputAction> PauseGameAction;
	
	/**
	 * 캐릭터 움직임 관련 변수 리스트
	 **/
private:
	UPROPERTY()
	FVector2D MoveInputValue;

	UPROPERTY()
	float MovementScale;
	
	UPROPERTY()
	float DefaultVelocityValue;

	UPROPERTY()
	float DefaultMaxVelocityValue;

	UPROPERTY()
	float SystemMaxVelocityValue;
	
	UPROPERTY()
	float CurrentMaxVelocityValue;
	
	UPROPERTY()
	TArray<float> MaxVelocityByGear;
	
	UPROPERTY()
	TArray<float> MaxVelocityMagnificationByGear;

	UPROPERTY()
	float AddJumpForceValue;
	
	FTimerHandle JumpDelayTimerHandle;
	
	FTimerHandle DropDownTimerHandle;
	
	FTimerHandle JustTimingTimerHandle;
	
	UPROPERTY()
	float RBD_JustTimingCheckTime;
	
	UPROPERTY()
	float JumpDelayTime;
	
	UPROPERTY()
	float VelocityDecelerateTime;

	UPROPERTY()
	FVector VelocityDecelerateTarget;

public:
	uint8 IsDropDownActive : 1 = false;
	
	/** 공격 관련 변수 리스트 \n
	* 약어 정리 \n
	* DA = DashAttack \n 
	* RB = ReBound \n
	* RBD = ReBoundDash \n
	**/
private:
	FTimerHandle DA_DurationTimerHandle;
	
	FTimerHandle AttackCoolDownTimerHandle;

	FTimerHandle DropDownHitCheckTimerHandle;
	
	FTimerHandle DropDownCoolDownTimerHandle;
	
	FCollisionQueryParams DA_Params;

	FCollisionQueryParams FD_Params;
	
	UPROPERTY()
	uint8 bCanDashOnFlying : 1;

	UPROPERTY()
	float DA_BaseDamage;

	UPROPERTY()
	TArray<float> DA_MultiplyDamageByGear;
	
	UPROPERTY()
	float DA_AddVelocityValue;
	
	UPROPERTY()
	float DA_DurationTime;

	UPROPERTY()
	float DA_ElapsedTime;
	
	UPROPERTY()
	float DA_DecelerateValue;
	
	UPROPERTY()
	float AttackCoolDownTime;

	UPROPERTY()
	float DropDownDamage;
	
	UPROPERTY()
	float DropDownVelocityValue;

	UPROPERTY()
	float DropDownMinimumHeightValue;

	UPROPERTY()
	float DropDownElapsedTime;
	
	UPROPERTY()
	float DropDownCoolDownTime;
	
	/**
	 *	플레이어 상태 관련 함수 리스트
	 **/
private:
	void CheckGearState();
	void PlayDeadAnim();
	void StartFadeOut();
	
	/**
	 * 플레이어 상태 관련 변수 리스트
	 **/
private:
	FTimerHandle CheckIdleStateTimerHandle;
	
	FTimerHandle CheckGearStateTimerHandle;

	FTimerHandle AccelerateTimerHandle;
	
	FTimerHandle DamageDelayTimerHandle;
	
	EGearState CurrentGearState;

	EGearState AttackInputGearState;
	
	TArray<FLinearColor> ColorsByGear;
	
	UPROPERTY()
	uint8 bIsDead : 1;
	
	UPROPERTY()
	uint8 bIsMoving : 1;
	
	UPROPERTY()
	uint8 bIsRolling : 1;

	UPROPERTY()
	uint8 bIsFlying : 1;

	UPROPERTY()
	uint8 bIsAccelerated : 1;
	
	UPROPERTY()
	uint8 bIsUsedFlyDash : 1;

	UPROPERTY()
	uint8 bIsMovingMustRolling : 1;

	UPROPERTY()
	uint8 bIsRollingIdleToWalk : 1;

	UPROPERTY()
	uint8 bIsReBounding : 1;

	UPROPERTY()
	uint8 bIsKnockBackOnGoing : 1;
	
	UPROPERTY()
	uint8 bIsInputJustAction : 1;
	
	UPROPERTY()
	uint8 bIsAttackOnGoing : 1;

	UPROPERTY()
	uint8 bIsDecelerateOnGoing : 1;
	
	/** 리바운드 관련 함수 리스트\n
	 * 캐릭터에서 너무 많은 기능을 수행해서는 안되므로 \n
	 * 가급적이면 리바운드는 충돌하는 대상에서 각도, 충격량 등을 계산하고 \n
	 * 리바운드 실행 함수에서는 기어에 따른 배율을 적용해 리바운드를 수행하는 역할만을 한다.
	 **/
public:
	void RB_ApplyReBoundByObjectType(FVector& ReBoundResultValue, EReBoundObjectType ObjectType);
	void RB_ApplyKnockBackByObjectType(FVector& ReBoundResultValue, EReBoundObjectType ObjectType);
	
private:
	void RB_CheckContactToFloor();
	void RBD_SuccessEvent();
	void RBD_FailedPenaltyEndEvent();
	
	/** 리바운드 관련 변수 리스트 \n
	 * ReBound를 RB_와 같은 형태로 축약해서 표현한다. \n
	 * ReBoundDash를 RBD_와 같은 형태로 축약해서 표현한다.
	**/
private:
	FTimerHandle RB_ContactCheckHandle;
	
	FTimerHandle RBD_JustTimingCheckHandle;

	FTimerHandle RBD_SucceedTimerHandle;
	
	FTimerHandle RBD_FailedTimerHandle;
	
	UPROPERTY()
	float RBD_AddVelocityValue;

	UPROPERTY()
	float RB_DisableMovementTime;
	
	UPROPERTY()
	TArray<float> RB_MultiplyValuesByGear;

	UPROPERTY()
	TArray<float> RB_MultiplyValuesByObjectType;

	// 타이머 델리게이트 등
private:
	void DA_HitCheckSequence();
	void FD_HitCheckSequence();
	void AttackCoolDownTimer();
	void DropDownCoolDownTimer();
	void BeginDeceleration();
	void VelocityDecelerateTimer();
	void EnhancedVelocityDecelerateSequence();
	void OnDecelerationEnd(bool ResetVelocity = true);
	void DamageDelayTimer();
	void AccelerationTimer();
};

