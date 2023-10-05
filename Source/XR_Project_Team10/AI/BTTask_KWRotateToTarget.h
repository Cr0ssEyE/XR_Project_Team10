// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_KWRotateToTarget.generated.h"

/*
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UBTTask_KWRotateToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_KWRotateToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
