﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XR_Project_Team10/CommonMonster/CommonMonster.h"
#include "CrowTalon.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API ACrowTalon : public ACommonMonster
{
	GENERATED_BODY()
	
public:	
	ACrowTalon();

	virtual void Tick(float DeltaTime) override;

	// IICommonMonsterBase을(를) 통해 상속
protected:
	virtual void BeginPlay() override;

	virtual void AttackOmen() override;
	virtual void Attack() override;
	virtual void AttackBehaviour() override;
	virtual void AttackEnd() override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	void AttackEndCheck();

	virtual void PlayDeadAnimation();
	virtual void PlayHitAnimation();

	// 외부 수정 변수
private:
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "공격 사거리")
	float AttackRange = 0.5;

	UPROPERTY(EditAnywhere, Category = RushAttack, DisplayName = "돌진 공격력")
	int RushPower = 1;

	UPROPERTY(EditAnywhere, Category = RushAttack, DisplayName = "돌진 속도")
	float RushSpeed = 0.5;


	// 내부 수정 변수
private:
	UPROPERTY()
	FVector OriPos;

	UPROPERTY()
	FVector RushVector;

	UPROPERTY()
	FTimerHandle RushTimerHandle;

	UPROPERTY()
	FVector AttackDir;

	UPROPERTY()
	FVector ReBoundVector;
	
	uint8 bIsDamageCaused : 1;
};
