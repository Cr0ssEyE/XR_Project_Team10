// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_KWBackDistanceCheck.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UBTDecorator_KWBackDistanceCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_KWBackDistanceCheck();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, DisplayName = "후방 체크 거리")
	float BackyardDistance;
};
