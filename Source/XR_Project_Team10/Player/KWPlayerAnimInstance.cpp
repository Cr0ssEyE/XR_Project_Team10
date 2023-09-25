// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Player/KWPlayerAnimInstance.h"
#include "GameFramework/Character.h"

UKWPlayerAnimInstance::UKWPlayerAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void UKWPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
}

void UKWPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Velocity = KiwiAnimDataAsset->Velocity;
	MoveSpeed = KiwiAnimDataAsset->MoveSpeed;
	MovingThreshold = KiwiAnimDataAsset->MovingThreshold;
	JumpingThreshold = KiwiAnimDataAsset->JumpingThreshold;
	bIsIdle = true;
	bIsJumping = false;
	bIsFalling = false;
}

void UKWPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
