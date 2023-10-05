// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_KWHohonuLunaticCheck.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UBTService_KWHohonuLunaticCheck : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_KWHohonuLunaticCheck();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	
};
