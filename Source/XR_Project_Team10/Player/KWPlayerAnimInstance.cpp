// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Player/KWPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UKWPlayerAnimInstance::UKWPlayerAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 50.0f;
}

void UKWPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	if(OwnerCharacter)
	{
		MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UKWPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Velocity = FVector::ZeroVector;
	MoveSpeed = 0.f;
	// MovingThreshold = KiwiAnimDataAsset->MovingThreshold;
	// JumpingThreshold = KiwiAnimDataAsset->JumpingThreshold;
	bIsIdle = true;
	bIsJumping = false;
	bIsFalling = false;
}

void UKWPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(MovementComponent)
	{
		Velocity = MovementComponent->Velocity;
		MoveSpeed = Velocity.Size2D();
		bIsIdle = MoveSpeed < MovingThreshold;
		bIsFalling = MovementComponent->IsFalling();
		bIsJumping = bIsFalling && (Velocity.Z > JumpingThreshold);
	}
}
