// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_KWMoveToNearby.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UBTTask_KWMoveToNearby : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UBTTask_KWMoveToNearby();
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere, DisplayName = "근접 체크 거리");
	float NearbyDistance;

	UPROPERTY(EditAnywhere, DisplayName = "설정 대상 키")
	FBlackboardKeySelector SubBlackboardKey;
};
