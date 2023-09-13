// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "KWPlayerCharacter.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API AKWPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

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

	// Player Input Function
protected:
	void LeftAddForceAction(const FInputActionValue& Value);
	void RightAddForceAction(const FInputActionValue& Value);
	void UpAddForceAction(const FInputActionValue& Value);
	void DownAddForceAction(const FInputActionValue& Value);
	void JumpAddForceAction(const FInputActionValue& Value);

	void ToggleCharacterTypeAction(const FInputActionValue& Value);
	void JumpJustTimingAction(const FInputActionValue& Value);
	void BoostAddForceAction();
	void EnableBoostCoolDownTimer();
	
	// Player Input Action
private:
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<class UInputAction> ToggleTypeAction;
	
	UPROPERTY()
	TObjectPtr<class UInputAction> LeftButtonAction;

	UPROPERTY()
	TObjectPtr<class UInputAction> RightButtonAction;

	UPROPERTY()
	TObjectPtr<class UInputAction>  UpButtonAction;
	
	UPROPERTY()
	TObjectPtr<class UInputAction>  DownButtonAction;
	
	UPROPERTY()
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY()
	TObjectPtr<class UInputAction> BoostAction;
	
private:
	UPROPERTY()
	float AddMoveForceValue;
	
	UPROPERTY()
	float MaxMoveForceValue;

	UPROPERTY()
	float AddJumpForceValue;
	
	UPROPERTY()
	FTimerHandle JumpDelayTimerHandle;

	UPROPERTY()
	FTimerHandle JustTimingTimerHandle;
	
	UPROPERTY()
	FTimerHandle StopCheckTimerHandle;
	
	UPROPERTY()
	float JumpDelayRemainTime;

	UPROPERTY()
	float JustTimingTime;
	
	UPROPERTY()
	float JustTimingElapsedTime;
	
	UPROPERTY()
	float JumpDelayTime;

	UPROPERTY()
	FTimerHandle BoostDurationTimerHandle;

	UPROPERTY()
	FTimerHandle BoostCoolDownTimerHandle;
	
	UPROPERTY()
	float BoostAddForceValue;

	UPROPERTY()
	float BoostAddMaxForceValue;
	
	UPROPERTY()
	float BoostCurrentRemainTime;
	
	UPROPERTY()
	float BoostDurationTime;

	UPROPERTY()
	float BoostCoolDownRemainTime;
	
	UPROPERTY()
	float BoostCoolDownTime;
	
private:
	uint32 bIsRolling : 1;

	uint32 bIsJumping : 1;
	
	uint32 bIsAttackOnGoing : 1;
};
