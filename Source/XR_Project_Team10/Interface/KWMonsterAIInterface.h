// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KWMonsterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UKWMonsterAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class XR_PROJECT_TEAM10_API IKWMonsterAIInterface
{
	GENERATED_BODY()
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetAIBaseLocation() = 0;
	virtual float GetAIMaximumMoveDistance() = 0;
	virtual float GetAIPatrolRadius() = 0;	//Ž�� (���ƴٴϴ� �� ũ��)
	virtual float GetAIDetectRange() = 0;	//���� (�÷��̾� ����)
	virtual float GetAIAttackRange() = 0;	//���� (�÷��̾� ����)
	virtual float GetAITurnSpeed() = 0;
};	//�������� -> ����� �ٽ� Ž��
