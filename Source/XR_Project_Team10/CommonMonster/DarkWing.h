// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XR_Project_Team10/CommonMonster/CommonMonster.h"
#include "DarkWing.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API ADarkWing : public ACommonMonster
{
	GENERATED_BODY()
public:
	ADarkWing();
	
	// IICommonMonsterBase을(를) 통해 상속
protected:
	virtual void BeginPlay() override;

	virtual void Research() override;
	virtual void Recognition() override;
	virtual void Tracking() override;
	virtual void AttackConfig() override;
	virtual void AttackOmen() override;
	virtual void Attack() override;
	virtual void Dead() override;
	
	// 변수
private:
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "공격 준비 시간")
	float AttackReadyTime;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "공격 사거리")
	float AttackRange;


	UPROPERTY(EditAnywhere, Category = FeatherAttack, DisplayName = "깃털 공격력")
	uint32 FeatherPower;

	UPROPERTY(EditAnywhere, Category = FeatherAttack, DisplayName = "깃털 속도")
	float FeatherSpeed;

	UPROPERTY(EditAnywhere, Category = FeatherAttack, DisplayName = "날리는 깃털 갯수")
	uint32 FeatherNum;


	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "깃털 삭제되는 시간")
	float FeatherDeleteTime;

	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "다음 공격 시간")
	float AttackNextTime;
};
