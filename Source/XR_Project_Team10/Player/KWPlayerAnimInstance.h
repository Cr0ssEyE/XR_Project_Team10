// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KWKiwiAnimDataAsset.h"
#include "Animation/AnimInstance.h"
#include "KWPlayerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FEndDeadAnimDelegate)

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UKWPlayerAnimInstance();

	FEndDeadAnimDelegate EndDeadAnimDelegate;
	
protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_DeadAnimEnd() { EndDeadAnimDelegate.Broadcast(); }
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Character")
	TObjectPtr<UKWKiwiAnimDataAsset> KiwiAnimDataAsset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class ACharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float JumpingThreshold;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsIdle : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsFalling : 1;
};
