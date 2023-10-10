// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_KWRandomValueCheck.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UBTTask_KWRandomValueCheck : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_KWRandomValueCheck();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, DisplayName = "설정 대상 키")
	FBlackboardKeySelector BlackboardKey;

	UPROPERTY(EditAnywhere, DisplayName = "랜덤 범위(Int)")
	int RandMax;
};
