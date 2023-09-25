// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CommonMonsterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UCommonMonsterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "���� ü��")
	int32 MonsterHP;

	UPROPERTY(EditAnywhere, Category = InputValue, DisplayName = "���� �̵��ӵ�")
	int32 MonsterMoveSpeed;


public:
	UPROPERTY(EditAnywhere, Category = Range, DisplayName = "���� Ž�� ���� ������ ��")
	float MonsterResearchRange;

	UPROPERTY(EditAnywhere, Category = Range, DisplayName = "���� ���� ���� ������ ��")
	float MonsterRecognitionRange;

	UPROPERTY(EditAnywhere, Category = Range, DisplayName = "���� ���� ���� ������ ��")
	float MonsterTrackingRange;

	UPROPERTY(EditAnywhere, Category = Range, DisplayName = "���� ���� ���� Ȯ�� ������ ��")
	float MonsterAttackConfigRange;

public:

	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "���� ��� ���� ���ӽð�")
	float MonsterIdleTime;

	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "���� Ž�� ���� ���ӽð�")
	float MonsterResearchTime;

	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "���� ���� ���� ���ӽð�")
	float MonsterRecognitionTime;

	UPROPERTY(EditAnywhere, Category = Time, DisplayName = "���� ���� ���� ���ӽð�")
	float MonsterTrackingTime;
};
