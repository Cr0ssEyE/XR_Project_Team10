// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "KWCommonAIController.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API AKWCommonAIController : public AAIController
{
	GENERATED_BODY()

public:
	AKWCommonAIController();

	void ActivateAI();
	void DeActivateAI();
	
	UFUNCTION()
	void SetTarget(const TArray<AActor*>& Actors);
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<UBlackboardData> CommonBlackboardData;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> CommonBehaviorTree;

	UPROPERTY()
	TObjectPtr<UAIPerceptionComponent> CommonPerceptionComp;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> CommonSight;

};
