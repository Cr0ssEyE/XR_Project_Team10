// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_KWRandomValueCheck.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UBTService_KWRandomValueCheck : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_KWRandomValueCheck();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere, DisplayName = "설정 대상 키")
	FBlackboardKeySelector BlackboardKey;

	UPROPERTY(EditAnywhere, DisplayName = "랜덤 범위(Int)")
	int RandMax;
};
