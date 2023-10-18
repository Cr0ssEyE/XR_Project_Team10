// Fill out your copyright notice in the Description page of Project Settings.

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

	// IICommonMonsterBase��(��) ���� ���
protected:
	virtual void BeginPlay() override;

	virtual void AttackOmen(AActor* Target) override;
	virtual void Attack(AActor* Target) override;

	// �ܺ� ���� ����
private:
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "���� �غ� �ð�")
	float AttackReadyTime = 0.5;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "���� ��Ÿ�")
	float AttackRange = 0.5;


	UPROPERTY(EditAnywhere, Category = RushAttack, DisplayName = "���� ���ݷ�")
	int RushPower = 0.5;

	UPROPERTY(EditAnywhere, Category = RushAttack, DisplayName = "���� �ӵ�")
	float RushSpeed = 0.5;


	// ���� ���� ����
private:
	UPROPERTY()
	FVector OriPos;

	UPROPERTY()
	FVector RushVector;

	UPROPERTY()
	FTimerHandle RushTimerHandle;
};
