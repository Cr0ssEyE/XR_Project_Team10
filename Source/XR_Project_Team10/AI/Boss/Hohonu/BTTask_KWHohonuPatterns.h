// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "XR_Project_Team10/Enumeration/KWHohonuPattern.h"
#include "BTTask_KWHohonuPatterns.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UBTTask_KWHohonuPatterns : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_KWHohonuPatterns();

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "실행 패턴")
	EHohonuPattern TaskPattern;
	
private:
	
};
