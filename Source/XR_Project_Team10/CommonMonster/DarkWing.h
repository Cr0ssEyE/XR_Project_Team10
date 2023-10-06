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
	
	// IICommonMonsterBase��(��) ���� ���
protected:
	virtual void BeginPlay() override;

	virtual void Research() override;
	virtual void Recognition() override;
	virtual void Tracking() override;
	virtual void AttackConfig() override;
	virtual void AttackOmen() override;
	virtual void Attack() override;
	virtual void Dead() override;
	
	// ����
private:
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "���� �غ� �ð�")
	float AttackReadyTime;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "���� ��Ÿ�")
	float AttackRange;


	UPROPERTY(EditAnywhere, Category = FeatherAttack, DisplayName = "���� ���ݷ�")
	uint32 FeatherPower;

	UPROPERTY(EditAnywhere, Category = FeatherAttack, DisplayName = "���� �ӵ�")
	float FeatherSpeed;

	UPROPERTY(EditAnywhere, Category = FeatherAttack, DisplayName = "������ ���� ����")
	uint32 FeatherNum;


	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "���� �����Ǵ� �ð�")
	float FeatherDeleteTime;

	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "���� ���� �ð�")
	float AttackNextTime;
};
