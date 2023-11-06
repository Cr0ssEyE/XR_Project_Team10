// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CommonMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UCommonMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCommonMonsterAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite ,Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsAttack;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;
};
