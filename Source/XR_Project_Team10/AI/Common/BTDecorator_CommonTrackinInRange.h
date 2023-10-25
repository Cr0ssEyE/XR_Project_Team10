// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CommonTrackinInRange.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UBTDecorator_CommonTrackinInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CommonTrackinInRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
