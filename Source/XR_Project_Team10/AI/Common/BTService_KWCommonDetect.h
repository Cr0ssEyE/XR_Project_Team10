// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_KWCommonDetect.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UBTService_KWCommonDetect : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_KWCommonDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
