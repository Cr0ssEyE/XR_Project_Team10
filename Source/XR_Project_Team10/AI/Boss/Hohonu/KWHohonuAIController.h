// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "KWHohonuAIController.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API AKWHohonuAIController : public AAIController
{
	GENERATED_BODY()

public:
	AKWHohonuAIController();

	void ActivateAI();
	void DeActivateAI();
	
	UFUNCTION()
	void SetTarget(const TArray<AActor*>& Actors);
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	UPROPERTY()
    TObjectPtr<UBlackboardData> BlackboardData;
    
    UPROPERTY()
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
};
